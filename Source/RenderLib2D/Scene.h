#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

#include"Object.h"

class HitResult
{
public:
    double m_Distance;
    std::shared_ptr<Material> m_Material;

public:
    HitResult() = default;
    ~HitResult() = default;
};

class Scene
{
protected:
    uint32_t m_Width;
    uint32_t m_Height;
    uint32_t m_Sample;

    std::vector<std::shared_ptr<Shape>> m_Shapes;

public:
    Scene() = default;
    ~Scene() = default;

public:
    void Init(uint32_t Width, uint32_t Height, uint32_t Sample)
    {
        m_Width = Width;
        m_Height = Height;

        m_Sample = Sample;

        m_Shapes.clear();

        std::shared_ptr<Emission> centerMaterial = std::make_shared<Emission>(Color(0.9, 0.9, 0.9));
        std::shared_ptr<Shape> centerShape = std::make_shared<Shape>(centerMaterial);
        centerShape->Add(std::make_shared<Capsule>(Point(200, 200), Point(400, 300), 50));

        std::shared_ptr<Glass> glassMaterial = std::make_shared<Glass>();
        std::shared_ptr<Shape> rightShape = std::make_shared<Shape>(glassMaterial);
        rightShape->Add(std::make_shared<Circle>(Point(500, 500), 100));
        rightShape->Add(std::make_shared<Circle>(Point(500, 550), 100));

        m_Shapes.push_back(centerShape);
        m_Shapes.push_back(rightShape);
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
    Color Sample(const Point& originPoint, int sIndex)
    {
        // Uniform Sample Lighting Path ?

        double randUnit = 2 * pi / m_Sample;
        double randDir = Uniform(0, randUnit) + randUnit * (sIndex - 1);

        return Trace(originPoint, Direction(cos(randDir), sin(randDir)), 64);
    }

    Color Trace(const Point& originPoint, const Direction& Dir, int Depth)
    {
        Point tracePoint(originPoint);
        Direction traceDir(Dir.normalized());

        while (Depth--)
        {
            auto nextStep = Hit(tracePoint);

            // Empty Scene
            if (!nextStep.m_Material)
            {
                return Color(0, 0, 0);
            }

            if (nextStep.m_Distance < eps)
            {
                if (nextStep.m_Material->getType() == mType::EMISSION)
                {
                    return ((Emission*)nextStep.m_Material.get())->getEmissionColor();
                }
                else if (nextStep.m_Material->getType() == mType::GLASS)
                {
                    return Color(0, 0, 0);
                }
            }
            else
            {
                tracePoint += traceDir * nextStep.m_Distance;
            }
        }

        return Color(0, 0, 0);
    }

protected:
    HitResult Hit(const Point& originPoint)
    {
        HitResult thisResult;
        thisResult.m_Distance = inf;
        thisResult.m_Material = nullptr;

        for (auto sIndex = m_Shapes.begin(); sIndex != m_Shapes.end(); ++sIndex)
        {
            auto thisDistance = (*sIndex)->SDF(originPoint);

            if (thisDistance < thisResult.m_Distance)
            {
                thisResult.m_Distance = thisDistance;
                thisResult.m_Material = (*sIndex)->getMaterial();
            }
        }

        return thisResult;
    }
};
