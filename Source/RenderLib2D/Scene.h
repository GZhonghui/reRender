#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

#include"Object.h"
#include"Light.h"

const uint32_t G_MAX_STEP = 128;
const uint32_t G_MAX_DEPTH = 3;
const double   G_MAX_DISTANCE = 1024;

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

        /*
        std::shared_ptr<Glass> centerMaterial = std::make_shared<Glass>(0.9);
        std::shared_ptr<Shape> centerShape = std::make_shared<Shape>(centerMaterial);
        centerShape->Add(std::make_shared<Capsule>(Point(200, 100), Point(200, 500), 20));

        std::shared_ptr<Glass> glassMaterial = std::make_shared<Glass>(0.9);
        std::shared_ptr<Shape> rightShape = std::make_shared<Shape>(glassMaterial);
        centerShape->Add(std::make_shared<Capsule>(Point(500, 100), Point(500, 400), 20));

        std::shared_ptr<Emission> otherMaterial = std::make_shared<Emission>(Color(1.2, 1.8, 2.8));
        std::shared_ptr<Shape> otherShape = std::make_shared<Shape>(otherMaterial);
        otherShape->Add(std::make_shared<Circle>(Point(650, 550), 50));

        m_Shapes.push_back(centerShape);
        m_Shapes.push_back(rightShape);
        m_Shapes.push_back(otherShape);
        */

        std::shared_ptr<Glass> centerMaterial = std::make_shared<Glass>(0.2, 1.5, Color(0.6, 0, 0));
        std::shared_ptr<Shape> centerShape = std::make_shared<Shape>(centerMaterial);
        centerShape->Add(std::make_shared<Box>(Point(400, 180), Direction(100, 50), pi * 0.2));

        std::shared_ptr<Glass> glassMaterial = std::make_shared<Glass>(0.2, 1.5, Color(0, 0.6, 0));
        std::shared_ptr<Shape> rightShape = std::make_shared<Shape>(glassMaterial);
        rightShape->Add(std::make_shared<Box>(Point(180, 360), Direction(100, 50), 0));

        std::shared_ptr<Emission> otherMaterial = std::make_shared<Emission>(Color(3, 3, 3));
        std::shared_ptr<Shape> otherShape = std::make_shared<Shape>(otherMaterial);
        otherShape->Add(std::make_shared<Circle>(Point(100, 100), 50));
        otherShape->Add(std::make_shared<Circle>(Point(500, 500), 50));

        m_Shapes.push_back(centerShape);
        m_Shapes.push_back(rightShape);
        m_Shapes.push_back(otherShape);
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
        // Debug Normal
        /*
        auto normalDir = Normal(originPoint);
        normalDir.x() = Limit(normalDir.x(), -1, 1) * 0.5 + 0.5;
        normalDir.y() = Limit(normalDir.y(), -1, 1) * 0.5 + 0.5;

        return Color(normalDir.x(), normalDir.y(), 0);
        */

        // Uniform Sample Lighting Path ?

        double randUnit = 2 * pi / m_Sample;
        double randDir = Uniform(0, randUnit) + randUnit * (sIndex - 1);

        return Trace(originPoint, Direction(cos(randDir), sin(randDir)), 1);
    }

protected:
    Color Trace(const Point& originPoint, const Direction& Dir,uint32_t usedDepth)
    {
        Point tracePoint(originPoint);
        Direction traceDir(Dir.normalized());

        uint32_t usedStep = 0;
        double   usedDistance = 0;

        double Sign = Hit(tracePoint).m_Distance > 0 ? 1 : -1;

        while (++usedStep <= G_MAX_STEP)
        {
            auto nextStep = Hit(tracePoint);

            // Empty Scene
            if (!nextStep.m_Material)
            {
                break;
            }

            usedDistance += nextStep.m_Distance * Sign;
            if (usedDistance > G_MAX_DISTANCE)
            {
                break;
            }

            // Hit Something at edge
            if (nextStep.m_Distance * Sign < eps)
            {
                if (nextStep.m_Material->getType() == mType::EMISSION)
                {
                    return ((Emission*)nextStep.m_Material.get())->getEmissionColor();
                }

                else if (nextStep.m_Material->getType() == mType::GLASS)
                {
                    Color Sum = ((Glass*)nextStep.m_Material.get())->getEmissionColor();

                    if (usedDepth < G_MAX_DEPTH)
                    {
                        auto Reflectivity = ((Glass*)nextStep.m_Material.get())->getReflectivity();
                        auto Eta = ((Glass*)nextStep.m_Material.get())->getEta();

                        // Really Need?
                        Reflectivity = Limit(Reflectivity, 0, 1);
                        Eta = Limit(Eta, 0.1, 10);

                        auto thisNormal = Normal(tracePoint);

                        Direction RefractDir;
                        if (Refract(traceDir, thisNormal * Sign, Sign > 0 ? 1 / Eta : Eta, RefractDir))
                        {
                            Sum += (1 - Reflectivity) * Trace(tracePoint - bias * Sign * thisNormal, RefractDir, usedDepth + 1);
                        }
                        else
                        {
                            Reflectivity = 1;
                        }

                        auto ReflectDir = Reflect(traceDir, thisNormal * Sign);

                        Sum += Reflectivity * Trace(tracePoint + thisNormal * Sign * bias, ReflectDir, usedDepth + 1);
                    }

                    return Sum.cwiseProduct(BeerLambert(((Glass*)nextStep.m_Material.get())->getAbsorption(), usedDistance));
                }
            }
            else
            {
                tracePoint += traceDir * nextStep.m_Distance * Sign;
            }
        }

        return Color(0, 0, 0);
    }

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

    Direction Normal(const Point& normalPoint)
    {
        Point Left, Right;

        Left = normalPoint + Point(eps, 0);
        Right = normalPoint - Point(eps, 0);
        double normalX = (Hit(Left).m_Distance - Hit(Right).m_Distance) * (0.5 / eps);

        Left = normalPoint + Point(0, eps);
        Right = normalPoint - Point(0, eps);
        double normalY = (Hit(Left).m_Distance - Hit(Right).m_Distance) * (0.5 / eps);

        return Direction(normalX, normalY);
    }
};
