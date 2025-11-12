#include "Renderer.hpp"
#include "Rgb.hpp"
#include "Ray.hpp"
#include "Constants.hpp"
#include <cmath>
#include <thread>

Renderer::Renderer(std::string file_path, int width, int height, Vec3 look_from, Vec3 look_at, Vec3 vup,
                   double focal_length, double vfov, SurfaceGroup& world)
    : image_width{width}, image_height{height}, look_from{look_from}, look_at{look_at}, vup{vup},
      focal_length{focal_length}, vfov{vfov}, world{world}
{
    pixel_samples_scale = 1.0 / samples_per_pixel;
    out_file.open(file_path);
    image.resize(image_height, std::vector<Rgb>(image_width));

    auto theta = degreesToRadians(vfov);
    auto h = std::tan(theta / 2);
    viewport_height = 2 * h * focal_length;
    viewport_width = viewport_height * (double(image_width) / image_height);
    w = unit_vector(look_from - look_at);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);
    Vec3 viewport_u = viewport_width * u;
    Vec3 viewport_v = viewport_height * (-1) * v;
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;
    auto viewport_upper_left = look_from - (focal_length * w) - viewport_u / 2 - viewport_v / 2;
    viewport_pixel_00_location = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
}

Vec3 Renderer::sampleSquare() const
{
    return Vec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
}

Ray Renderer::getRay(int i, int j) const
{
    auto offset = sampleSquare();
    auto pixel_sample = viewport_pixel_00_location + ((i + offset.x()) * pixel_delta_u) + (
                            (j + offset.y()) * pixel_delta_v);
    auto ray_origin = look_from;
    auto ray_direction = pixel_sample - ray_origin;
    return Ray(ray_origin, ray_direction);
}

Rgb Renderer::shade(const Ray& ray, const HitRecord& hit) const
{
    Rgb color(0, 0, 0);
    const double kd = 0.8;
    const double ks = 0.2;
    const double shininess = 32.0;

    for(const auto& ambient: ambient_lights)
    {
        color += hit.material->color * ambient.color * ambient.intensity;
    }

    Vec3 v = unit_vector((-1) * ray.direction());

    for(const auto& light: point_lights)
    {
        Vec3 light_dir = unit_vector(light.position - hit.point);
        Ray shadow_ray(hit.point + hit.normal * 0.001, light_dir);
        HitRecord shadow_rec;
        if(!world.hit(shadow_ray, Interval(0.001, infinity), shadow_rec))
        {
            double diffuse = std::max(0.0, dot(hit.normal, light_dir));
            Vec3 h = unit_vector(light_dir + v);
            double specular = std::pow(std::max(0.0, dot(hit.normal, h)), shininess);
            color += hit.material->color * light.color * light.intensity * (diffuse * kd + specular * ks);
        }
    }
    return color;
}

Rgb Renderer::rayColor(const Ray& r)
{
    HitRecord rec;
    if(world.hit(r, Interval(0.001, infinity), rec))
    {
        return shade(r, rec);
    }
    Vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * Rgb(1.0, 1.0, 1.0) + a * Rgb(0.5, 0.7, 1.0);
}

// //no multithreading
// void Renderer::render()
// {
//     for(int j = 0; j < image_height; j++)
//     {
//         for(int i = 0; i < image_width; i++)
//         {
//             Rgb pixel_color(0,0,0);
//             for(int sample = 0; sample < samples_per_pixel; sample++)
//             {
//                 Ray r = getRay(i, j);
//                 pixel_color += rayColor(r);
//             }
//             image[j][i] = pixel_samples_scale * pixel_color;
//         }
//     }
// }

//for multithreading
void Renderer::renderChunk(int start_row, int end_row)
{
    for(int j = start_row; j < end_row; j++)
    {
        for(int i = 0; i < image_width; i++)
        {
            Rgb pixel_color(0, 0, 0);
            for(int sample = 0; sample < samples_per_pixel; sample++)
            {
                Ray r = getRay(i, j);
                pixel_color += rayColor(r);
            }
            image[j][i] = pixel_samples_scale * pixel_color;
        }
    }
}

//with multithreading
void Renderer::render()
{
    int num_threads = 8;
    int chunk_height = image_height / num_threads;
    std::vector<std::thread> threads;
    for(int t = 0; t < num_threads; t++)
    {
        int start = t * chunk_height;
        int end = (t == num_threads - 1) ? image_height : start + chunk_height;
        threads.emplace_back(&Renderer::renderChunk, this, start, end);
    }
    for(auto& th: threads)
    {
        th.join();
    }
}

void Renderer::color(std::ostream& out, const Rgb& pixel)
{
    auto r = pixel.r();
    auto g = pixel.g();
    auto b = pixel.b();

    static const Interval intensity(0.000, 0.999);

    int ir = static_cast<int>(255.999 * intensity.clamp(r));
    int ig = static_cast<int>(255.999 * intensity.clamp(g));
    int ib = static_cast<int>(255.999 * intensity.clamp(b));
    out << ir << " " << ig << " " << ib << "\n";
}

void Renderer::imageToPpm()
{
    out_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for(int j = 0; j < image_height; j++)
    {
        for(int i = 0; i < image_width; i++)
        {
            color(out_file, image[j][i]);
        }
    }
}
