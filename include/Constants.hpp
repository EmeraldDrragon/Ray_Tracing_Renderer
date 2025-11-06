#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cmath>
#include <cstdlib>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;
const double kEpsilon = 1e-8;

inline double degreesToRadians(double degrees)
{
    return degrees * pi / 180;
}

inline double randomDouble()
{
    return std::rand() / (RAND_MAX + 1.0);
}

inline double randomDouble(double min, double max)
{
    return min + (max - min) * randomDouble();
}

#endif //CONSTANTS_HPP
