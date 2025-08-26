#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

class Vec3
{
private:
    double x_c;
    double y_c;
    double z_c;
public:
    Vec3() : x_c{0}, y_c{0}, z_c{0} {}
    Vec3(double x, double y, double z) : x_c{x}, y_c{y}, z_c{z} {}
    double x() const { return x_c; }
    double y() const { return y_c; }
    double z() const { return z_c; }

    Vec3& operator+=(const Vec3& v)
    {
        x_c += v.x_c;
        y_c += v.y_c;
        z_c += v.z_c;
        return *this;
    }

    Vec3& operator*=(double t)
    {
        x_c *= t;
        y_c *= t;
        z_c *= t;
        return *this;
    }

    Vec3& operator/=(double t)
    {
        return *this *= 1 / t;
    }

    double length() const
    {
        return std::sqrt(x_c*x_c + y_c*y_c + z_c*z_c);
    }
};

inline std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline Vec3 operator+(const Vec3& v, const Vec3& w)
{
    return {v.x() + w.x(), v.y() + w.y(), v.z() + w.z()};
}

inline Vec3 operator-(const Vec3& v, const Vec3& w)
{
    return {v.x()-w.x(), v.y()-w.y(), v.z()-w.z()};
}

inline Vec3 operator*(double t, const Vec3& v)
{
    return {t * v.x(), t * v.y(), t * v.z()};
}

inline Vec3 operator*(const Vec3& v, double t)
{
    return t * v;
}

inline Vec3 operator/(const Vec3& v, double t)
{
    return (1/t) * v;
}

inline double dot(const Vec3& v, const Vec3& w)
{
    return v.x() * w.x() + v.y() * w.y() + v.z() * w.z();
}

inline Vec3 cross(const Vec3& v, const Vec3& w)
{
    return {v.y()*w.z() - v.z()*w.y(),
                v.z()*w.x() - v.x()*w.z(),
                v.x()*w.y() - v.y()*w.x()};
}

inline Vec3 unit_vector(const Vec3& v)
{
    return v / v.length();
}

#endif //VEC3_H
