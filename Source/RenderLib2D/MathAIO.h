#pragma once

#include<Eigen/Eigen>

typedef Eigen::Vector2d Point;
typedef Eigen::Vector2d Direction;
typedef Eigen::Vector3d Color;

const double eps = static_cast<double>(1e-4);
const double inf = std::numeric_limits<double>::infinity();

const double pi = std::acos(-1.0);

inline double Uniform()
{
#define RANDOM_MAX_LIMIT (1024)

    return 1.0 * (rand() % RANDOM_MAX_LIMIT) / (RANDOM_MAX_LIMIT - 1);
}

inline double Uniform(double Left, double Right)
{
    return Left + Uniform() * (Right - Left);
}
