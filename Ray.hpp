#ifndef RAY_HPP
#define RAY_HPP

#include "Vec3.hpp"

class Ray
{
private:
    Vec3 o;
    Vec3 d;

public:
    Ray() {}
    Ray(const Vec3& o, const Vec3& d) : o(o), d(d) {}
    const Vec3& origin() const { return o; }
    const Vec3& direction() const { return d; }

    Vec3 evaluate(double t) const
    {
        return o + t * d;
    }
};


#endif //RAY_HPP
