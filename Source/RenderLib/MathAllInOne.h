#ifndef MATH_ALL_IN_ONE_H
#define MATH_ALL_IN_ONE_H

#pragma warning(disable:26451)

#include<algorithm>
#include<cmath>

#include<Eigen/Dense>

typedef Eigen::Vector3d Point;
typedef Eigen::Vector3d Direction;
typedef Eigen::Vector3d Color;
typedef Eigen::Vector2d TextureCoords;
typedef Eigen::Vector3i TriangleFace;

const Point WorldCenter(0, 0, 0);

//Right Hand
const Direction WorldRight(1, 0, 0);
const Direction WorldForward(0, 1, 0);
const Direction WorldUp(0, 0, 1);

const Color colorRed(1, 0, 0);
const Color colorGreen(0, 1, 0);
const Color colorBlue(0, 0, 1);
const Color colorBlack(0, 0, 0);
const Color colorWhite(1, 1, 1);

const Color colorGray(0.547, 0.638, 0.638);

const double eps = static_cast<double>(1e-6);
const double inf = std::numeric_limits<double>::infinity();

const double pi = std::acos(-1.0);

inline double UniformRandom()
{
	const int rand_limit = 10240;
	return 1.0 * (rand() % rand_limit) / (rand_limit - 1);
}

inline double UniformRandom(double min,double max)
{
	return min + (max - min) * UniformRandom();
}

inline double DegreesToRadians(double Degrees)
{
	return Degrees * pi / 180;
}

inline double RadiansToDegrees(double Radians)
{
	return Radians * 180 / pi;
}

inline double Clamp(double x, double min, double max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

inline double Lerp(double value, double left, double right)
{
	return left + (right - left) * value;
}

inline Point SampleSpherePoint()
{
	Direction Res;

	do
	{
		Res[0] = UniformRandom(-1, 1);
		Res[1] = UniformRandom(-1, 1);
		Res[2] = UniformRandom(-1, 1);
	} while (Res.norm() > 1);

	return Res;
}

inline Direction SampleSphereDirection()
{
	return SampleSpherePoint().normalized();
}

inline Direction SampleHalfSphereDirection(const Direction& Normal)
{
	auto Res = SampleSphereDirection();
	
	if (Res.dot(Normal) < 0)
	{
		Res = -Res;
	}

	return Res;
}

#endif