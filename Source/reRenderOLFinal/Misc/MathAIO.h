#ifndef MATHAIO_H
#define MATHAIO_H

#include <cmath>
#include <cstdint>

#include <Eigen/Eigen>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>

// const double pi = acos(-1.0);

const double eps = 1e-6;

typedef Eigen::Vector3d Direction;
typedef Eigen::Vector3d Point;
typedef Eigen::Vector3d Color;

typedef Eigen::Vector2d TextureCoords;

typedef Eigen::Vector3i TriangleFace;

const Color colorGray(105 / 255.999, 105 / 255.999, 105 / 255.999);
const Color colorGreen(32 / 255.999, 127 / 255.999, 76 / 255.999);
const Color colorBlack(0, 0, 0);

class Transform
{
public:
    Transform()
    {
        Location = Eigen::Vector3d(0,0,0);
        Rotation = Eigen::Vector3d(0,0,0);
        Scale = Eigen::Vector3d(1,1,1);
    }
    Transform(const Eigen::Vector3d& inLocation, const Eigen::Vector3d& inRotation, const Eigen::Vector3d& inScale)
    {
        Location = inLocation;
        Rotation = inRotation;
        Scale    = inScale;
    }
    ~Transform() = default;

public:
    Eigen::Vector3d Location;
    Eigen::Vector3d Rotation;
    Eigen::Vector3d Scale;
};

inline void Convert(const Eigen::Vector3d& From, glm::vec3& To)
{
    To[0] = From.x();
    To[1] = From.y();
    To[2] = From.z();
}

inline void Convert(const glm::vec3& From, Eigen::Vector3d& To)
{
    To.x() = From[0];
    To.y() = From[1];
    To.z() = From[2];
}

inline glm::vec3 Convert(const Eigen::Vector3d& From)
{
    return glm::vec3(From.x(), From.y(), From.z());
}

inline Eigen::Vector3d Convert(const glm::vec3& From)
{
    return Eigen::Vector3d(From[0], From[1], From[2]);
}

inline void Fill(float* Target, const Eigen::Vector3d& Source)
{
    Target[0] = Source.x();
    Target[1] = Source.y();
    Target[2] = Source.z();
}

inline void Fill(Eigen::Vector3d& Target, const float* Source)
{
    Target.x() = Source[0];
    Target.y() = Source[1];
    Target.z() = Source[2];
}

inline bool Near(double x, double y, double localEPS = eps)
{
    return std::abs(x - y) < localEPS;
}

inline double UniformRandom()
{
    const int rand_limit = 10240;
    return 1.0 * (rand() % rand_limit) / (rand_limit - 1);
}

inline double UniformRandom(double min,double max)
{
    return min + (max - min) * UniformRandom();
}

inline double Clamp(double x, double min, double max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

#endif // MATHAIO_H
