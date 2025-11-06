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

int main()
{
    //ppm file
    std::string ppm_file_path = "image.ppm";

    //image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    //camera
    Vec3 look_from = Vec3(3, 3, 3);
    Vec3 look_at = Vec3(0, 0, -1);
    Vec3 vup(0, 1, 0);
    auto focal_length = 1.0;
    auto vfov = 40.0;

    Material default_material(Rgb(31.0 / 255.0, 254.0 / 255.0, 183.0 / 255.0));
    Material ground_material(Rgb(0.5, 0.5, 0.5));

    SurfaceGroup world;

    auto torus = Mesh("Ball.obj", make_shared<Material>(default_material));
    world.add(make_shared<Mesh>(torus));
    world.add(make_shared<Sphere>(Vec3(0.0, -101, -1.0), 100.0, make_shared<Material>(ground_material)));
    // world.add(make_shared<Sphere>(Vec3( 0.0,    0.0, -1.2),   0.5,make_shared<Material>(default_material)));
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
    render_inst.addPointLight(PointLight(Vec3(-1,1,2), Rgb(1, 1, 1), 1));
    render_inst.addAmbientLight(AmbientLight(Rgb(1, 1, 1), 0.1));

    render_inst.render();
    render_inst.imageToPpm();
}
