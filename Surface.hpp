#ifndef SURFACE_HPP
#define SURFACE_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include "Vec3.hpp"
#include "Rgb.hpp"
#include "Ray.hpp"
#include <string>
#include <memory>
#include <vector>
#include "Constants.hpp"
#include "Interval.hpp"

using std::make_shared;
using std::shared_ptr;

class Material
{
public:
    Rgb color;
    Material(const Rgb& col) : color{col} {}
};

class HitRecord
{
public:
    Vec3 point;
    Vec3 normal;
    double t;
    bool front_face;
    shared_ptr<Material> material;

    void setFaceNormal(const Ray& r, const Vec3& outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : (-1) * outward_normal;
    }
};

class Surface
{
public:
    virtual ~Surface() = default;

    virtual bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const = 0;
};

//class for world - a group of different surfaces - 3d objects
class SurfaceGroup : public Surface
{
public:
    std::vector<shared_ptr<Surface>> objects;
    SurfaceGroup() {}
    SurfaceGroup(shared_ptr<Surface> object) { add(object); }
    void clear() { objects.clear(); }
    void add(shared_ptr<Surface> object) { objects.push_back(object); }

    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;
};

class Sphere : public Surface
{
private:
    Vec3 center;
    double radius;
    shared_ptr<Material> material;

public:
    Sphere(const Vec3& center, double radius, shared_ptr<Material> mat)
        : center{center}, radius{std::fmax(0, radius)}, material{mat} {}

    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;
};

class Triangle : public Surface
{
public:
    Vec3 v0;
    Vec3 v1;
    Vec3 v2;
    Vec3 n0;
    Vec3 n1;
    Vec3 n2;

    Triangle(Vec3 v0, Vec3 v1, Vec3 v2, Vec3 n0, Vec3 n1, Vec3 n2, shared_ptr<Material> mat)
        : v0{v0}, v1{v1}, v2{v2}, n0{n0}, n1{n1}, n2{n2} {}

    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;
};

class Mesh : public Surface
{
private:
    std::vector<Triangle> triangles;
    shared_ptr<Material> material;

public:
    Mesh(std::string obj_file_path, shared_ptr<Material> mat);

    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override;
};


#endif //SURFACE_HPP
