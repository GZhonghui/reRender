#pragma once

#include"MathAllInOne.h"

class Ray
{
public:
	Ray() = default;
	Ray(const Point& origin,const Direction& forward):
		m_origin(origin), m_forward(forward.normalized()){}

	~Ray() = default;

protected:
	Point m_origin;
	Direction m_forward;

public:
	Point At(double length) const
	{
		return m_origin + m_forward * length;
	}

	const Point& origin() const
	{
		return m_origin;
	}

	const Direction& forward() const
	{
		return m_forward;
	}
};