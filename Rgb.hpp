//
// Created by srdja on 7/29/2025.
//

#ifndef RGB_H
#define RGB_H

#include <iostream>

class Rgb
{
private:
    double red;
    double green;
    double blue;
public:
    Rgb() : red{0}, green{0}, blue{0} {}
    Rgb(double r, double g, double b) : red{r}, green{g}, blue{b} {}
    double r() const { return red; }
    double g() const { return green; }
    double b() const { return blue; }

    void r(double value)
    {
        red = value;
    }
    void g(double value)
    {
        green = value;
    }
    void b(double value)
    {
        blue = value;
    }

    Rgb& operator+=(const Rgb& v)
    {
        red += v.red;
        green += v.green;
        blue += v.blue;
        return *this;
    }

    Rgb& operator*=(double t)
    {
        red *= t;
        green *= t;
        blue *= t;
        return *this;
    }

    Rgb& operator/=(double t)
    {
        return *this *= 1 / t;
    }
};

inline std::ostream& operator<<(std::ostream& out, const Rgb& v)
{
    return out << v.r() << ' ' << v.g() << ' ' << v.b();
}

inline Rgb operator+(const Rgb& v, const Rgb& w)
{
    return {v.r() + w.r(), v.g() + w.g(), v.b() + w.b()};
}

inline Rgb operator-(const Rgb& v, const Rgb& w)
{
    return {v.r()-w.r(), v.g()-w.g(), v.b()-w.b()};
}

inline Rgb operator*(double t, const Rgb& v)
{
    return {t * v.r(), t * v.g(), t * v.b()};
}

inline Rgb operator*(const Rgb& v, double t)
{
    return t * v;
}

inline Rgb operator/(const Rgb& v, double t)
{
    return (1/t) * v;
}

inline double dot(const Rgb& v, const Rgb& w)
{
    return v.r() * w.r() + v.g() * w.g() + v.b() * w.b();
}

inline Rgb cross(const Rgb& v, const Rgb& w)
{
    return {v.g()*w.b() - v.b()*w.g(),
                v.b()*w.r() - v.r()*w.b(),
                v.r()*w.g() - v.g()*w.r()};
}

inline Rgb operator*(const Rgb& v, const Rgb& w)
{
    return {v.r() * w.r(), v.g() * w.g(), v.b() * w.b()};
}



#endif //RGB_H
