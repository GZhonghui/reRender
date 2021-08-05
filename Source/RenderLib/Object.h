#pragma once

#include<memory>
#include<cstdint>

#include"Ray.h"
#include"BBox.h"
#include"Model.h"
#include"Material.h"
#include"Intersection.h"
#include"MathAllInOne.h"

class Object
{
public:
	Object() = default;
	~Object() = default;

public:
	std::shared_ptr<Material> m_Material;

public:
	virtual double Area() const = 0;
	virtual Point Center() const = 0;
	virtual Point Sample() const = 0;
	virtual bool GenerateBox(BoundingBox& Box) const = 0;
	virtual bool Intersect(const Ray& ray, Intersection& HitRes) = 0;
	virtual TextureCoords HitPointToTextureCoords(double U, double V) const = 0;
};

class Sphere :public Object
{
public:
	Sphere():
		m_Center(0,0,0), Radius(1){}
	Sphere(const Point& center,double Radius):
		m_Center(center), Radius(Radius){}
	~Sphere() = default;

public:
	Point m_Center;
	double Radius;

public:
	virtual double Area() const override;
	virtual Point Center() const override;
	virtual Point Sample() const override;
	virtual bool GenerateBox(BoundingBox& Box) const override;
	virtual bool Intersect(const Ray& ray, Intersection& HitRes) override;
	virtual TextureCoords HitPointToTextureCoords(double U, double V) const override;
};

class Triangle :public Object
{
public:
	Triangle():m_VerticesPtr(nullptr){}
	Triangle(uint32_t A, uint32_t B, uint32_t C,
		std::vector<std::shared_ptr<Vertex>>* VerticesPtr):
		m_VerticesPtr(VerticesPtr),A(A),B(B),C(C){}
	~Triangle() = default;

public:
	uint32_t A, B, C;
	std::vector<std::shared_ptr<Vertex>>* m_VerticesPtr;

public:
	virtual double Area() const override;
	virtual Point Center() const override;
	virtual Point Sample() const override;
	virtual bool GenerateBox(BoundingBox& Box) const override;
	virtual bool Intersect(const Ray& ray, Intersection& HitRes) override;
	virtual TextureCoords HitPointToTextureCoords(double U, double V) const override;
};