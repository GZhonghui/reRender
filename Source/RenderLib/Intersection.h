#pragma once

#include<memory>

#include"MathAllInOne.h"

#pragma warning(disable:26495)

class Object;
class Material;

class Intersection
{
public:
	Intersection()
	{
		Hit = false;
		HitDistance = inf;
	}
	~Intersection() = default;

public:
	bool Hit;
	bool HitForward;
	double HitDistance;
	Point HitPoint;
	double U;
	double V;
	Direction HitNormal;
	Object* HitObject;
	std::shared_ptr<Material> HitMaterial;
};