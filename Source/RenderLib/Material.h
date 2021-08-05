#pragma once

#include"Ray.h"
#include"Texture.h"
#include"Intersection.h"
#include"MathAllInOne.h"

enum class MaterialType
{
	EMISSION,
	DIFFUSE,
	GLASS,
	GLOSSY
};

//Base Class
class Material
{
public:
	Material(MaterialType Type):
		m_Type(Type),baseColor(colorWhite) {}
	~Material() = default;

protected:
	MaterialType m_Type;

public:
	inline MaterialType GetType() const
	{
		return m_Type;
	}

public:
	Color baseColor;

public:
	//Back to Light Source via Light Path
	virtual bool Scatter(const Ray& inRay, const Intersection& HitRes, Ray& NextRay) const = 0;
};

//Emission
class Emission :public Material
{
public:
	Emission() :
		Material(MaterialType::EMISSION),
		Strength(1) {}
	~Emission() = default;

public:
	double Strength;

public:
	virtual bool Scatter(const Ray& inRay, const Intersection& HitRes, Ray& NextRay) const;
};

//Diffuse
class Diffuse :public Material
{
public:
	Diffuse() :
		Material(MaterialType::DIFFUSE) {}
	~Diffuse() = default;

public:
	std::shared_ptr<Texture> diffuseTexture;

public:
	virtual bool Scatter(const Ray& inRay, const Intersection& HitRes, Ray& NextRay) const;
};

//Glass
class Glass :public Material
{
public:
	Glass() :
		Material(MaterialType::GLASS),
		Roughness(0),IOR(2) {}
	~Glass() = default;

public:
	double Roughness;
	//Index of Refraction
	double IOR;

public:
	virtual bool Scatter(const Ray& inRay, const Intersection& HitRes, Ray& NextRay) const;
};

//Glossy
class Glossy :public Material
{
public:
	Glossy() :
		Material(MaterialType::GLOSSY),
		Roughness(0) {}
	~Glossy() = default;

public:
	double Roughness;

public:
	virtual bool Scatter(const Ray& inRay, const Intersection& HitRes, Ray& NextRay) const;
};