#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Vec3.hpp"
#include "Rgb.hpp"

class Light
{
public:
    Rgb color;
    double intensity;
    Light(const Rgb& col, double i) : color{col}, intensity{i} {}
    virtual ~Light() = default;
};

class PointLight : public Light
{
public:
    Vec3 position;
    PointLight(const Vec3& pos, const Rgb& col, double i) : Light(col, i), position{pos} {}
};

class AmbientLight : public Light
{
public:
    AmbientLight(const Rgb& col, double i) : Light(col, i) {}
};

#endif //LIGHT_HPP
