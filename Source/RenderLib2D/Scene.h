#pragma once

#include"MathAIO.h"
#include"ToolAIO.h"

#include"Object.h"
#include"Light.h"

const uint32_t G_MAX_STEP = 128;
const uint32_t G_MAX_DEPTH = 4;
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

        /*
        std::shared_ptr<Glass> centerMaterial = std::make_shared<Glass>(0.2, 1.5, Color(0.6, 0, 0));
        std::shared_ptr<Shape> centerShape = std::make_shared<Shape>(centerMaterial);
        centerShape->Add(std::make_shared<Box>(Point(400, 180), Direction(100, 50), pi * 0.2));

        std::shared_ptr<Glass> glassMaterial = std::make_shared<Glass>(0.2, 1.5, Color(0, 0.6, 0));
        std::shared_ptr<Shape> rightShape = std::make_shared<Shape>(glassMaterial);
        //rightShape->Add(std::make_shared<Box>(Point(180, 360), Direction(100, 50), 0));
        rightShape->Add(std::make_shared<Capsule>(Point(120, 360), Point(240, 360), 60));

        std::shared_ptr<Emission> otherMaterial = std::make_shared<Emission>(Color(3, 3, 3));
        std::shared_ptr<Shape> otherShape = std::make_shared<Shape>(otherMaterial);
        otherShape->Add(std::make_shared<Circle>(Point(100, 100), 50));
        otherShape->Add(std::make_shared<Circle>(Point(500, 500), 50));

        m_Shapes.push_back(centerShape);
        m_Shapes.push_back(rightShape);
        m_Shapes.push_back(otherShape);
        */

        // Light
        {
            std::shared_ptr<Emission> thisMaterial = std::make_shared<Emission>(Color(2, 2, 2));
            std::shared_ptr<Shape> thisShape = std::make_shared<Shape>(thisMaterial);
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(512, 48), Direction(420, 12), 0, 10));

            thisShape->Add(std::make_shared<Capsule>(Point(160, 520), Point(360, 520), 20));
            thisShape->Add(std::make_shared<Capsule>(Point(1024 - 360, 520), Point(1024 - 160, 520), 20));

            m_Shapes.push_back(thisShape);
        }

        // r
        {
            std::shared_ptr<Glass> thisMaterial = std::make_shared<Glass>(0.2, 1.5, Color(0, 0.6, 0.9));
            std::shared_ptr<Shape> thisShape = std::make_shared<Shape>(thisMaterial);
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(60, 310), Direction(10, 60), 0, 6));

            m_Shapes.push_back(thisShape);
        }

        {
            std::shared_ptr<Emission> thisMaterial = std::make_shared<Emission>(Color(1.2, 1.2, 1.2));
            std::shared_ptr<Shape> thisShape = std::make_shared<Shape>(thisMaterial);
            thisShape->Add(std::make_shared<Circle>(Point(90, 270), 20));

            m_Shapes.push_back(thisShape);
        }

        // e
        {
            std::shared_ptr<Glass> thisMaterial = std::make_shared<Glass>(0.2, 1.5, Color(0.7, 0, 0.9));
            std::shared_ptr<Shape> thisShape = std::make_shared<Shape>(thisMaterial);
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(135, 310), Direction(10, 60), 0, 6));
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(175, 310), Direction(40, 10), 0, 6));
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(175, 260), Direction(40, 10), 0, 6));
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(175, 360), Direction(40, 10), 0, 6));
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(205, 290), Direction(10, 30), 0, 6));

            m_Shapes.push_back(thisShape);
        }

        // R
        {
            std::shared_ptr<Glass> thisMaterial = std::make_shared<Glass>(0.2, 1.5, Color(0, 0, 0));
            std::shared_ptr<Shape> thisShape = std::make_shared<Shape>(thisMaterial);
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(260, 280), Direction(10, 120), 0, 6));
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(320, 170), Direction(60,  10), 0, 6));
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(320, 270), Direction(60,  10), 0, 6));
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(370, 220), Direction(10,  50), 0, 6));

            m_Shapes.push_back(thisShape);
        }

        {
            std::shared_ptr<Emission> thisMaterial = std::make_shared<Emission>(Color(1, 1.2, 1.2));
            std::shared_ptr<Shape> thisShape = std::make_shared<Shape>(thisMaterial);
            thisShape->Add(std::make_shared<Capsule>(Point(295, 305), Point(385, 395), 14));

            m_Shapes.push_back(thisShape);
        }

        // e
        {
            std::shared_ptr<Glass> thisMaterial = std::make_shared<Glass>(0.2, 1.5, Color(0.6, 0.6, 0));
            std::shared_ptr<Shape> thisShape = std::make_shared<Shape>(thisMaterial);
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(425, 310), Direction(10, 60), 0, 6));
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(465, 310), Direction(40, 10), 0, 6));
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(465, 260), Direction(40, 10), 0, 6));
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(465, 360), Direction(40, 10), 0, 6));
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(495, 290), Direction(10, 30), 0, 6));

            m_Shapes.push_back(thisShape);
        }

        // n
        {
            std::shared_ptr<Glass> thisMaterial = std::make_shared<Glass>(0.2, 1.5, Color(0, 0, 0.7));
            std::shared_ptr<Shape> thisShape = std::make_shared<Shape>(thisMaterial);
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(550, 310), Direction(10, 60), 0, 6));
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(630, 310), Direction(10, 60), 0, 6));
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(590, 260), Direction(40, 10), 0, 6));

            m_Shapes.push_back(thisShape);
        }

        // d
        {
            std::shared_ptr<Glass> thisMaterial = std::make_shared<Glass>(0.2, 1.5, Color(0.6, 0, 0));
            std::shared_ptr<Shape> thisShape = std::make_shared<Shape>(thisMaterial);
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(680, 310), Direction(10,  60), 0, 6));
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(760, 270), Direction(10, 100), 0, 6));
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(710, 260), Direction(40,  10), 0, 6));
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(710, 360), Direction(40,  10), 0, 6));

            m_Shapes.push_back(thisShape);
        }

        // e
        {
            std::shared_ptr<Glass> thisMaterial = std::make_shared<Glass>(0.2, 1.5, Color(0.1, 0, 0.1));
            std::shared_ptr<Shape> thisShape = std::make_shared<Shape>(thisMaterial);
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(815, 310), Direction(10, 60), 0, 6));
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(855, 310), Direction(40, 10), 0, 6));
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(855, 260), Direction(40, 10), 0, 6));
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(855, 360), Direction(40, 10), 0, 6));
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(885, 290), Direction(10, 30), 0, 6));

            m_Shapes.push_back(thisShape);
        }

        // r
        {
            std::shared_ptr<Glass> thisMaterial = std::make_shared<Glass>(0.2, 1.5, Color(0, 0.6, 0));
            std::shared_ptr<Shape> thisShape = std::make_shared<Shape>(thisMaterial);
            thisShape->Add(std::make_shared<BoxWithFillet>(Point(940, 310), Direction(10, 60), 0, 6));

            m_Shapes.push_back(thisShape);
        }

        {
            std::shared_ptr<Emission> thisMaterial = std::make_shared<Emission>(Color(1.4, 1.2, 1.2));
            std::shared_ptr<Shape> thisShape = std::make_shared<Shape>(thisMaterial);
            thisShape->Add(std::make_shared<Circle>(Point(970, 270), 20));

            m_Shapes.push_back(thisShape);
        }
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

        // Debug Mode
        /*
        if (Hit(originPoint).m_Distance < 0)
        {
            return Color(1, 1, 1);
        }
        else
        {
            return Color(0, 0, 0);
        }
        */

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

                    if (Sign > 0)
                    {
                        return Sum;
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
