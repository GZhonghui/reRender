#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

#include"Material.h"

enum class oType
{
    LINE, SEGMENT, CAPSULE, TRIANGLE, BOX, CIRCLE
};

class Object
{
protected:
    oType m_Type;

public:
    Object(oType Type) : m_Type(Type) {}
    virtual ~Object() = default;

public:
    oType getType() const noexcept { return m_Type; }

public:
    virtual double SDF(const Point& originPoint) = 0;
};

class Line : public Object
{
protected:
    Point m_PointOnLine;
    Direction m_Normal;

public:
    Line(const Point& PointOnLine, const Direction& Normal) :
        Object(oType::LINE), m_PointOnLine(PointOnLine), m_Normal(Normal.normalized()) {}
    virtual ~Line() = default;

public:
    virtual double SDF(const Point& originPoint)
    {
        return (originPoint - m_PointOnLine).dot(m_Normal);
    }
};

class Segment : public Object
{
protected:
    Point m_PointA;
    Point m_PointB;

public:
    Segment(const Point& PointA, const Point& PointB) :
        Object(oType::SEGMENT), m_PointA(PointA), m_PointB(PointB) {}
    virtual ~Segment() = default;

public:
    virtual double SDF(const Point& originPoint)
    {
        auto U = m_PointB - m_PointA;
        auto V = originPoint - m_PointA;
        auto pX = m_PointA + Limit(V.dot(U) / U.dot(U), 0, 1) * U;
        
        return (originPoint - pX).norm();
    }
};

class Capsule : public Object
{
protected:
    Point m_PointA;
    Point m_PointB;
    double m_Radius;

public:
    Capsule(const Point& PointA, const Point& PointB, double Radius) :
        Object(oType::CAPSULE), m_PointA(PointA), m_PointB(PointB), m_Radius(Radius) {}
    virtual ~Capsule() = default;

public:
    virtual double SDF(const Point& originPoint)
    {
        return Segment(m_PointA, m_PointB).SDF(originPoint) - m_Radius;
    }
};

class Triangle : public Object
{
protected:
    Point m_PointA;
    Point m_PointB;
    Point m_PointC;

public:
    Triangle(const Point& PointA, const Point& PointB, const Point& PointC) :
        Object(oType::TRIANGLE), m_PointA(PointA), m_PointB(PointB), m_PointC(PointC) {}
    virtual ~Triangle() = default;

public:
    virtual double SDF(const Point& originPoint)
    {
        // CODE HERE
        return 0;
    }
};

class Box : public Object
{
protected:
    Point m_Center;
    Direction m_HalfLength;
    double m_Theta;

public:
    Box(const Point& Center, const Direction& HalfLength, double Theta) :
        Object(oType::BOX), m_Center(Center), m_HalfLength(HalfLength), m_Theta(Theta) {}
    virtual ~Box() = default;

public:
    virtual double SDF(const Point& originPoint)
    {
        auto sinTheta = std::sin(m_Theta);
        auto cosTheta = std::cos(m_Theta);

        double dx = abs((originPoint.x() - m_Center.x()) * cosTheta + (originPoint.y() - m_Center.y()) * sinTheta) - m_HalfLength.x();
        double dy = abs((originPoint.y() - m_Center.y()) * cosTheta - (originPoint.x() - m_Center.x()) * sinTheta) - m_HalfLength.y();
        double ax = std::max(dx, 0.0);
        double ay = std::max(dy, 0.0);

        return std::min(std::max(dx, dy), 0.0) + std::sqrt(ax * ax + ay * ay);
    }
};

class Circle : public Object
{
protected:
    Point m_Center;
    double m_Radius;

public:
    Circle(const Point& Center, double Radius) :
        Object(oType::CIRCLE), m_Center(Center), m_Radius(Radius) {}
    virtual ~Circle() = default;

public:
    virtual double SDF(const Point& originPoint)
    {
        auto Offset = m_Center - originPoint;

        return Offset.norm() - m_Radius;
    }
};

class Shape
{
protected:
    std::vector<std::shared_ptr<Object>> m_SubObjects;
    std::shared_ptr<Material> m_Material;

public:
    Shape(std::shared_ptr<Material> shapeMaterial) :m_Material(shapeMaterial) {}
    ~Shape() = default;

public:
    void Add(std::shared_ptr<Object> newObject)
    {
        m_SubObjects.push_back(newObject);
    }

    double SDF(const Point& originPoint)
    {
        double sdfDistance = inf;

        for (auto oIndex = m_SubObjects.begin(); oIndex != m_SubObjects.end(); ++oIndex)
        {
            sdfDistance = std::min(sdfDistance, (*oIndex)->SDF(originPoint));
        }

        return sdfDistance;
    }

    std::shared_ptr<Material> getMaterial() const noexcept
    {
        return m_Material;
    }
};