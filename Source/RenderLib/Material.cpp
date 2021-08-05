#include"Material.h"

//Emission Dont Scatter
bool Emission::Scatter(const Ray& inRay, const Intersection& HitRes, Ray& NextRay) const
{
	return false;
}

bool Diffuse::Scatter(const Ray& inRay, const Intersection& HitRes, Ray& NextRay) const
{
	const double DiffuseEPS = 0.01;

	auto NextDir = SampleHalfSphereDirection(HitRes.HitNormal);
	if (NextDir.dot(HitRes.HitNormal) < DiffuseEPS)
	{
		NextDir = HitRes.HitNormal;
	}

	//Add A Offset
	NextRay = Ray(HitRes.HitPoint + HitRes.HitNormal * DiffuseEPS, NextDir);

	return true;
}

bool Glass::Scatter(const Ray& inRay, const Intersection& HitRes, Ray& NextRay) const
{
	//All Objects are separated
	const double GlassEPS = 0.01;

	double NoutOverNin;
	double Cosine;
	//Out -> In
	if (HitRes.HitForward)
	{
		NoutOverNin = 1.0 / IOR;
		Cosine = -inRay.forward().dot(HitRes.HitNormal);
	}
	//In -> Out
	else
	{
		NoutOverNin = IOR;
		Cosine = -IOR * inRay.forward().dot(HitRes.HitNormal);
	}

	//Refract
	bool haveRefract = false;
	Direction RefractDir;
	double dotNL = HitRes.HitNormal.dot(inRay.forward());
	double Discriminant = 1 - NoutOverNin * NoutOverNin * (1 - dotNL * dotNL);
	if (Discriminant > 0)
	{
		haveRefract = true;
		RefractDir = NoutOverNin * (inRay.forward() - HitRes.HitNormal * dotNL);
		RefractDir = RefractDir - std::sqrt(Discriminant) * HitRes.HitNormal;
	}

	//Reflect
	Direction ReflectDir = (2 * HitRes.HitNormal + inRay.forward());

	double P = UniformRandom();

	double R0 = (1 - IOR) / (1 + IOR);
	R0 = R0 * R0;
	double ReflectProb = R0 + (1 - R0) * std::pow(1 - Cosine, 5);

	// Do Refract
	if (haveRefract && P > ReflectProb)
	{
		NextRay = Ray(HitRes.HitPoint - HitRes.HitNormal * GlassEPS, RefractDir);
		return true;
	}
	//Do Reflect
	else
	{
		if (HitRes.HitNormal.dot(ReflectDir) > GlassEPS)
		{
			NextRay = Ray(HitRes.HitPoint + HitRes.HitNormal * GlassEPS, ReflectDir);
			return true;
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool Glossy::Scatter(const Ray& inRay, const Intersection& HitRes, Ray& NextRay) const
{
	const double GlossyMaxR = 0.5;
	const double GlossyEPS = 0.01;

	Direction NextDir = (2 * HitRes.HitNormal + inRay.forward());
	
	if (Roughness > GlossyEPS)
	{
		double RoughnessSphereR = Lerp(Clamp(Roughness, 0, 1), 0, GlossyMaxR);
		NextDir += SampleSphereDirection() * RoughnessSphereR;
		NextDir.normalize();
	}

	if (HitRes.HitNormal.dot(NextDir) > GlossyEPS)
	{
		NextRay = Ray(HitRes.HitPoint + HitRes.HitNormal * GlossyEPS, NextDir);
		return true;
	}

	return false;
}