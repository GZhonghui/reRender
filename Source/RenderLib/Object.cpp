#include"Object.h"

double Sphere::Area() const
{
    return 4 * pi * Radius * Radius;
}

Point Sphere::Center() const
{
    return m_Center;
}
Point Sphere::Sample() const
{
    //[-1,1]
    double x = UniformRandom() * 2 - 1;
    double y = UniformRandom() * 2 - 1;
    double z = UniformRandom() * 2 - 1;
    return m_Center + Direction(x, y, z).normalized() * Radius;
}

bool Sphere::GenerateBox(BoundingBox& Box) const
{
    Eigen::Vector3d Half(Radius + eps, Radius + eps, Radius + eps);
    Point corner_min = m_Center - Half;
    Point corner_max = m_Center + Half;
    Box = BoundingBox(corner_min, corner_max);

    return true;
}

bool Sphere::Intersect(const Ray& ray, Intersection& HitRes)
{
    //From Internet
    auto OC = ray.origin() - m_Center;
    auto A = ray.forward().dot(ray.forward());
    auto B = 2.0 * OC.dot(ray.forward());
    auto C = OC.dot(OC) - Radius * Radius;
    auto Discriminant = B * B - 4 * A * C;

    if (Discriminant < 0) return false;

    auto halfB = B * 0.5;
    auto halfSqrtDiscriminant = std::sqrt(Discriminant) * 0.5;

    auto root = (-halfB - halfSqrtDiscriminant) / A;
    if (root < eps)
    {
        root = (-halfB + halfSqrtDiscriminant) / A;
        if (root < eps)
        {
            return false;
        }
        else
        {
            //HitForward
            HitRes.HitForward = false;
        }
    }
    else
    {
        HitRes.HitForward = true;
    }

    HitRes.Hit = true;
    HitRes.HitDistance = root;
    HitRes.HitPoint = ray.At(HitRes.HitDistance);
    //Sphere Dont Support UV
    HitRes.U = 0;
    HitRes.V = 0;
    HitRes.HitNormal = (HitRes.HitForward ? 1 : -1) * (HitRes.HitPoint - m_Center) / Radius;
    HitRes.HitObject = static_cast<Object*>(this);
    HitRes.HitMaterial = m_Material;

    return true;
}

TextureCoords Sphere::HitPointToTextureCoords(double U, double V) const
{
    //Sphere Dont Support UV
    return TextureCoords(0, 0);
}

double Triangle::Area() const
{
    const Point& aLocation = (*m_VerticesPtr)[A]->m_Location;
    const Point& bLocation = (*m_VerticesPtr)[B]->m_Location;
    const Point& cLocation = (*m_VerticesPtr)[C]->m_Location;

    Direction AB = bLocation - aLocation;
    Direction AC = cLocation - aLocation;
    
    return AB.cross(AC).norm() * 0.5;
}

Point Triangle::Center() const
{
    const Point& aLocation = (*m_VerticesPtr)[A]->m_Location;
    const Point& bLocation = (*m_VerticesPtr)[B]->m_Location;
    const Point& cLocation = (*m_VerticesPtr)[C]->m_Location;

    Point target = (bLocation + cLocation) / 2;
    Direction AO = target - aLocation;
    return aLocation + AO * (2.0 / 3);
}

Point Triangle::Sample() const
{
    const Point& aLocation = (*m_VerticesPtr)[A]->m_Location;
    const Point& bLocation = (*m_VerticesPtr)[B]->m_Location;
    const Point& cLocation = (*m_VerticesPtr)[C]->m_Location;

    double u = UniformRandom();
    double v = UniformRandom();
    double w = UniformRandom();
    double sum = u + v + w;
    u = u / sum;
    v = v / sum;
    w = 1 - u - v;
    return aLocation * u + bLocation * v + cLocation * w;
}

bool Triangle::GenerateBox(BoundingBox& Box) const
{
    using std::min;
    using std::max;

    const Point& aLocation = (*m_VerticesPtr)[A]->m_Location;
    const Point& bLocation = (*m_VerticesPtr)[B]->m_Location;
    const Point& cLocation = (*m_VerticesPtr)[C]->m_Location;

    Point corner_min, corner_max;
    corner_min.x() = min(aLocation.x(), min(bLocation.x(), cLocation.x())) - eps;
    corner_min.y() = min(aLocation.y(), min(bLocation.y(), cLocation.y())) - eps;
    corner_min.z() = min(aLocation.z(), min(bLocation.z(), cLocation.z())) - eps;
    corner_max.x() = max(aLocation.x(), max(bLocation.x(), cLocation.x())) + eps;
    corner_max.y() = max(aLocation.y(), max(bLocation.y(), cLocation.y())) + eps;
    corner_max.z() = max(aLocation.z(), max(bLocation.z(), cLocation.z())) + eps;
    Box = BoundingBox(corner_min, corner_max);

    return true;
}

bool Triangle::Intersect(const Ray& ray, Intersection& HitRes)
{
    const Point& aLocation = (*m_VerticesPtr)[A]->m_Location;
    const Point& bLocation = (*m_VerticesPtr)[B]->m_Location;
    const Point& cLocation = (*m_VerticesPtr)[C]->m_Location;

    const Direction& aNormal = (*m_VerticesPtr)[A]->m_Normal;
    const Direction& bNormal = (*m_VerticesPtr)[B]->m_Normal;
    const Direction& cNormal = (*m_VerticesPtr)[C]->m_Normal;

    Direction AC = cLocation - aLocation;
    Direction AB = bLocation - aLocation;
    Direction AO = ray.origin() - aLocation;

    //Not the Normal info from Model File
    Direction faceNormal = AB.cross(AC).normalized();
    if (ray.forward().dot(faceNormal) > 0)
    {
        faceNormal = -faceNormal;
    }

    double t, u, v, inv = 1.0 / ray.forward().cross(AC).dot(AB);
    u = ray.forward().cross(AC).dot(AO) * inv;
    v = AO.cross(AB).dot(ray.forward()) * inv;
    t = AO.cross(AB).dot(AC) * inv;

    //u -> Point B
    //v -> Point C
    //w -> Point A
    double w = 1 - u - v;
    if (u < 0 || u>1) return false;
    if (v < 0 || v>1) return false;
    if (w < 0 || w>1) return false;
    if (t < eps) return false;

    //From Vertex
    Direction lerpNormal = w * aNormal + u * bNormal + v * cNormal;

    if (lerpNormal.dot(ray.forward()) < 0)
    {
        //HitForward
        HitRes.HitForward = true;
    }
    else
    {
        HitRes.HitForward = false;
    }

    HitRes.Hit = true;
    HitRes.HitDistance = t;
    HitRes.HitPoint = ray.At(t);
    HitRes.U = u;
    HitRes.V = v;
    HitRes.HitNormal = faceNormal;
    HitRes.HitObject = static_cast<Object*>(this);
    HitRes.HitMaterial = m_Material;

    return true;
}

TextureCoords Triangle::HitPointToTextureCoords(double U, double V) const
{
    const TextureCoords& aCoord = (*m_VerticesPtr)[A]->m_TextureCoords;
    const TextureCoords& bCoord = (*m_VerticesPtr)[B]->m_TextureCoords;
    const TextureCoords& cCoord = (*m_VerticesPtr)[C]->m_TextureCoords;

    double W = 1 - U - V;
    return aCoord * W + bCoord * U + cCoord * V;
}