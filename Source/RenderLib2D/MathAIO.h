#pragma once

#include<Eigen/Eigen>

typedef Eigen::Vector2d Point;
typedef Eigen::Vector2d Direction;
typedef Eigen::Vector3d Color;

const double eps = static_cast<double>(1e-4);
const double inf = std::numeric_limits<double>::infinity();

const double pi = std::acos(-1.0);