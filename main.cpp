#include <iostream>
#include <fstream>
#include "Vec3.hpp"
#include "Renderer.hpp"
#include "Rgb.hpp"
#include "Ray.hpp"
#include <string>
#include "Surface.hpp"
#include "Interval.hpp"
#include "Constants.hpp"

void readFace(Vec3& vertex, Triangle& triangle, std::string& token, std::vector<Vec3>& vertices, std::vector<Vec3>& normals)
{
    std::istringstream stream(token);
    std::string v;
    std::string t;
    std::string n;

    std::getline(stream, v, '/');
    std::getline(stream, t, '/');
    std::getline(stream, n, '/');

    int v_index = std::stoi(v);
    int n_index = std::stoi(n);

    vertex = vertices[v_index-1];
    triangle.normal = normals[n_index-1];
}

void mesh(SurfaceGroup& world, std::string obj_file_path, shared_ptr<Material> mat)
{
    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;
    std::ifstream file(obj_file_path);
    std::string line;
    while (std::getline(file, line))
    {
        if(line.empty() || line[0] == '#' || line.substr(0,7) == "mtllib") continue;

        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if(prefix == "v")
        {
            double x;
            double y;
            double z;
            iss >> x >> y >> z;
            vertices.push_back(Vec3(x,y,z));
        }
        if(prefix == "vn")
        {
            double x;
            double y;
            double z;
            iss >> x >> y >> z;
            normals.push_back(Vec3(x,y,z));
        }
        if(prefix == "f")
        {
            std::string v1;
            std::string v2;
            std::string v3;
            iss >> v1 >> v2 >> v3;
            auto triangle = make_shared<Triangle>(Vec3(0,0,0),Vec3(0,0,0),Vec3(0,0,0),Vec3(0,0,0), mat);

            readFace(triangle->v0,*triangle,v1,vertices,normals);
            readFace(triangle->v1,*triangle,v2,vertices,normals);
            readFace(triangle->v2,*triangle,v3,vertices,normals);

            world.add(triangle);
        }
    }
}


int main()
{
    //ppm file
    std::string ppm_file_path = "C:\\Users\\Salamat\\Desktop\\Salamat\\Programming\\Projects\\C++\\Ray_Tracing_Renderer\\image.ppm";

    //image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    //camera
    Vec3 look_from = Vec3(-2,3,3);
    Vec3 look_at = Vec3(0,0,-1);
    Vec3 vup(0,1,0);
    auto focal_length = 1.0;
    auto vfov = 90.0;

    Material default_material(Rgb(31.0/255.0,254.0/255.0,183.0/255.0));
    Material ground_material(Rgb(0.5, 0.5, 0.5));

    SurfaceGroup world;

    // mesh(world,"C:\\Salamat\\Ray-Tracing-redo\\Cylinder.obj",make_shared<Material>(default_material));

    world.add(make_shared<Sphere>(Vec3( 0.0, -101, -1.0), 100.0,make_shared<Material>(ground_material)));
    world.add(make_shared<Sphere>(Vec3( 0.0,    0.0, -1.2),   0.5,make_shared<Material>(default_material)));
    // world.add(make_shared<Sphere>(Vec3(-1.0,    0.0, -1.0),   0.5,make_shared<Material>(default_material)));
    // world.add(make_shared<Sphere>(Vec3(-1.0,    0.0, -1.0),   0.4,make_shared<Material>(default_material)));
    // world.add(make_shared<Sphere>(Vec3( 1.0,    0.0, -1.0),   0.5,make_shared<Material>(default_material)));


    // world.add(make_shared<Sphere>(Vec3(0,0,-1),0.5,make_shared<Material>(default_material)));
    // world.add(make_shared<Sphere>(Vec3(0,-100.5,-1),100,make_shared<Material>(ground_material)));
    //something aint right with the triangle fucking hell
    // world.add(make_shared<Triangle>(Vec3(0.418046, -0.591655, 0.714344), Vec3( -0.574449, 0.869435, -0.956081), Vec3(-1.132863, 0.164627, 0.830373), Vec3(0.4194, 0.7920, 0.4436),make_shared<Material>(default_material)));
    // world.add(make_shared<Triangle>(Vec3(0.418046, -0.591655, 0.714344), Vec3( 0.069791, -0.272578, -1.418910), Vec3(-0.574449, 0.869435, -0.956081), Vec3(0.8579, 0.5098, -0.0638),make_shared<Material>(default_material)));


    // world.add(make_shared<Sphere>(Vec3(-std::cos(pi/4), 0, -1), std::cos(pi/4), make_shared<Material>(default_material)));
    // world.add(make_shared<Sphere>(Vec3( std::cos(pi/4), 0, -1), std::cos(pi/4), make_shared<Material>(default_material)));

    Renderer render_inst(ppm_file_path, image_width, image_height, look_from, look_at, vup, focal_length, vfov, world);
    render_inst.addPointLight(PointLight(Vec3(-1,1,1), Rgb(1,1,1),1.0));
    render_inst.addAmbientLight(AmbientLight(Rgb(1,1,1),0.1));

    render_inst.render();
    render_inst.imageToPpm();
}