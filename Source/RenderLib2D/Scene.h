#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

#include"Object.h"

class Scene
{
protected:
    uint32_t m_Width;
    uint32_t m_Height;

    std::vector<std::shared_ptr<Shape>> m_Shapes;

public:
    Scene() = default;
    ~Scene() = default;

public:
    void Init(uint32_t Width, uint32_t Height)
    {
        m_Width = Width;
        m_Height = Height;

        m_Shapes.clear();
    }

    void Destroy()
    {
        m_Shapes.clear();
    }

    void Add(std::shared_ptr<Shape> newShape)
    {
        m_Shapes.push_back(newShape);
    }

    void Render(const char* filePath);

public:
    Color Sample(double x, double y)
    {

    }

    Color Trace(const Point& originPoint, const Direction& Dir, int Depth)
    {

    }
};
