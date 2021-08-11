#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

enum class oType
{
    LINE, SEGMENT, CAPSULE, TRIANGLE, CIRCLE
};

class Object
{
protected:
    oType m_Type;

public:
    Object(oType Type) : m_Type(Type) {}
    virtual ~Object() = default;

public:
    virtual oType getType() const noexcept { return m_Type; }

public:
    virtual double SDF(const Point& originPoint) = 0;
};

class Line : public Object
{
protected:
    Point m_PointOnLine;
    Direction m_Dir;

public:
    Line(const Point& PointOnLine, const Direction& Dir) :
        Object(oType::LINE), m_PointOnLine(PointOnLine), m_Dir(Dir) {}
    virtual ~Line() = default;

public:
    virtual double SDF(const Point& originPoint)
    {
        return 0;
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
        return 0;
    }
};

class Capsule : public Object
{
protected:
    Point m_PointA;
    Point m_PointB;

public:
    Capsule(const Point& PointA, const Point& PointB) :
        Object(oType::CAPSULE), m_PointA(PointA), m_PointB(PointB) {}
    virtual ~Capsule() = default;

public:
    virtual double SDF(const Point& originPoint)
    {
        return 0;
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

public:
    virtual double SDF(const Point& originPoint)
    {
        return 0;
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
        return 0;
    }
};

class Shape
{
protected:
    std::vector<std::shared_ptr<Object>> m_SubObjects;

public:
    Shape() = default;
    ~Shape() = default;

public:
    void Add(std::shared_ptr<Object> newObject)
    {
        m_SubObjects.push_back(newObject);
    }
};