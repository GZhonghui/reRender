#pragma once

#include"MathAllInOne.h"

//Camera UP is Always with World UP(Z)

class Camera
{
public:
	Camera()
	{
		m_Location = Point(0, 0, 0);
		m_Forward = Direction(0, 1, 0);
		m_Right = Direction(1, 0, 0);
		m_Up = Direction(0, 0, 1);
	}
	~Camera() = default;

protected:
	Point m_Location;
	Direction m_Forward;
	Direction m_Right;
	Direction m_Up;
	
	//Horizontal
	double FOV;
	//Width/Height
	double AspectRatio;
	double Focal;

public:
	void ReLocate(const Point& newLocation)
	{
		m_Location = newLocation;
	}

	void SetView(double FOV, double AspectRatio)
	{
		this->FOV = FOV;
		this->AspectRatio = AspectRatio;

		double half = DegreesToRadians(FOV * 0.5);
		Focal = 0.5 * AspectRatio / std::tan(half);
	}

	void UpdateWithDirection(const Direction& forward)
	{
		m_Forward = forward.normalized();

		m_Right = m_Forward.cross(WorldUp).normalized();
		m_Up = m_Right.cross(m_Forward).normalized();
	}

	void LookAt(const Point& target)
	{
		UpdateWithDirection(target - m_Location);
	}

	//Left -> Right;Up-> Down;[0,1]
	//Like Scanlines
	Ray Sample(double x, double y)
	{
		Direction ViewDirection = m_Forward * Focal;
		ViewDirection += m_Up * Lerp(y, 0.5, -0.5);
		ViewDirection += m_Right * Lerp(x, -0.5 * AspectRatio, 0.5 * AspectRatio);

		return Ray(m_Location, ViewDirection);
	}
	
	//(x,y) start at 1
	Ray Sample(int x, int y, int width, int height)
	{
		double halfWidthStep = 0.5 / width;
		double halfHeightStep = 0.5 / height;

		double xValue = ((x << 1) - 1) * halfWidthStep;
		double yValue = ((y << 1) - 1) * halfHeightStep;

		return Sample(xValue, yValue);
	}
};