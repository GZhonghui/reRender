#pragma once

//Function BoundingBox::Hit
#pragma warning(disable:26451)

#include"MathAllInOne.h"
#include"Ray.h"

class BoundingBox
{
public:
	BoundingBox() = default;
	BoundingBox(const Point& corner_min, const Point& corner_max) :
		corner_min(corner_min), corner_max(corner_max) {}
	BoundingBox(const BoundingBox& boxA, const BoundingBox& boxB)
	{
		using std::min;
		using std::max;

		corner_min[0] = min(boxA.corner_min[0], boxB.corner_min[0]);
		corner_min[1] = min(boxA.corner_min[1], boxB.corner_min[1]);
		corner_min[2] = min(boxA.corner_min[2], boxB.corner_min[2]);
		corner_max[0] = max(boxA.corner_max[0], boxB.corner_max[0]);
		corner_max[1] = max(boxA.corner_max[1], boxB.corner_max[1]);
		corner_max[2] = max(boxA.corner_max[2], boxB.corner_max[2]);
	}

	~BoundingBox() = default;
protected:
	Point corner_min;
	Point corner_max;

public:
	bool Hit(const Ray& ray)
	{
		using std::min;
		using std::max;
		using std::abs;

		double t_min = -inf, t_max = inf, t1, t2, inv;

		//3 Directions

		if (abs(ray.forward().x()) > eps)
		{
			inv = 1.0 / ray.forward().x();
			t1 = (corner_min.x() - ray.origin().x()) * inv;
			t2 = (corner_max.x() - ray.origin().x()) * inv;
			t_min = max(t_min, min(t1, t2));
			t_max = min(t_max, max(t1, t2));
		}

		if (abs(ray.forward().y()) > eps)
		{
			inv = 1.0 / ray.forward().y();
			t1 = (corner_min.y() - ray.origin().y()) * inv;
			t2 = (corner_max.y() - ray.origin().y()) * inv;
			t_min = max(t_min, min(t1, t2));
			t_max = min(t_max, max(t1, t2));
		}

		if (abs(ray.forward().z()) > eps)
		{
			inv = 1.0 / ray.forward().z();
			t1 = (corner_min.z() - ray.origin().z()) * inv;
			t2 = (corner_max.z() - ray.origin().z()) * inv;
			t_min = max(t_min, min(t1, t2));
			t_max = min(t_max, max(t1, t2));
		}

		if (t_min > t_max) return false;
		return true;
	}
};