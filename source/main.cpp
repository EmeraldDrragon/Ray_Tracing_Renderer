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
    Vec3 look_from = Vec3(-3, -1, 3);
    Vec3 look_at = Vec3(0, 0, -1);
    Vec3 vup(0, 1, 0);
    auto focal_length = 1.0;
    auto vfov = 90.0;

    //materials
    Material default_material(Rgb(31.0 / 255.0, 254.0 / 255.0, 183.0 / 255.0));
    Material ground_material(Rgb(0.5, 0.5, 0.5));

    //world
    SurfaceGroup world;

    //objects
    auto torus = Mesh("Torus.obj", make_shared<Material>(default_material));
    world.add(make_shared<Mesh>(torus));
    world.add(make_shared<Sphere>(Vec3(0.0, -101, -1.0), 100.0, make_shared<Material>(ground_material)));

    //renderer
    Renderer render_inst(ppm_file_path, image_width, image_height, look_from, look_at, vup, focal_length, vfov, world);

    //lights
    render_inst.addPointLight(PointLight(Vec3(-1,1,2), Rgb(1, 1, 1), 1));
    render_inst.addAmbientLight(AmbientLight(Rgb(1, 1, 1), 0.1));

    //render
    render_inst.render();
    render_inst.imageToPpm();
}
