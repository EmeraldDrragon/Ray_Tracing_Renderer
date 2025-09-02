#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <iostream>
#include <fstream>
#include "Vec3.hpp"
#include "Rgb.hpp"
#include <vector>
#include "Ray.hpp"
#include "Surface.hpp"
#include "Interval.hpp"
#include "Constants.hpp"
#include "Light.hpp"


class Renderer
{
private:
    //output file
    std::ofstream out_file;

    //image
    int image_width;
    int image_height;
    std::vector<std::vector<Rgb>> image;

    //camera
    double focal_length;
    double vfov;
    Vec3 look_from;
    Vec3 look_at;
    Vec3 vup;
    Vec3 u;
    Vec3 v;
    Vec3 w;

    //ssaa
    double pixel_samples_scale;

    //viewport
    double viewport_height;
    double viewport_width;
    Vec3 pixel_delta_u;
    Vec3 pixel_delta_v;
    Vec3 viewport_pixel_00_location;

    //world
    SurfaceGroup world;

    //lights
    std::vector<PointLight> point_lights;
    std::vector<AmbientLight> ambient_lights;

public:
    //ssaa
    int samples_per_pixel = 10;

    //constructor
    Renderer(std::string file_path, int width, int height, Vec3 look_from, Vec3 look_at, Vec3 vup, double focal_length, double vfov, SurfaceGroup& world);

    Vec3 sampleSquare() const;

    Ray getRay(int i, int j) const;

    Rgb shade(const Ray& ray, const HitRecord& hit) const;

    Rgb rayColor(const Ray& r);

    void renderChunk(int start_row, int end_row);

    void render();

    void color(std::ostream& out, const Rgb& pixel);

    void imageToPpm();

    void addPointLight(const PointLight& light) { point_lights.push_back(light); }

    void addAmbientLight(const AmbientLight& light) { ambient_lights.push_back(light); }
};


#endif //RENDERER_HPP
