#pragma once

class RenderConfig
{
public:
	RenderConfig()
	{
		//Default Config
		spp = 2;
		core = 2;
		windowWidth = 1024;
		windowHeight = 768;
		HorizontalAngle = 120;
		cameraPosX = 0;
		cameraPosY = 0;
		cameraPosZ = 0;
		targetPosX = 100;
		targetPosY = 0;
		targetPosZ = 0;
		enableMSAA = false;
		enableGammaCorrection = false;
	}
	~RenderConfig() = default;

public:
	int spp;
	int core;
	int windowWidth;
	int windowHeight;
	int HorizontalAngle;
	double cameraPosX;
	double cameraPosY;
	double cameraPosZ;
	double targetPosX;
	double targetPosY;
	double targetPosZ;
	bool enableMSAA;
	bool enableGammaCorrection;
};