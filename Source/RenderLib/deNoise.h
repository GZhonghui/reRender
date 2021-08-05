#pragma once

#include<algorithm>
#include<cstdint>
#include<memory>

#include<OpenImageDenoise/config.h>
#include<OpenImageDenoise/oidn.hpp>

#include"MathAllInOne.h"
#include"ToolAllInOne.h"

class deNoiser
{
public:
	deNoiser()
	{
		inMap = nullptr;
		colorMap = nullptr;
		normalMap = nullptr;
	}
	~deNoiser() = default;

public:
	float* inMap;
	float* colorMap;
	float* normalMap;
	int imageWidth;
	int imageHeight;
	//Default 3 channels

public:
	bool deNoise(uint8_t* outImage)
	{
		if (!inMap) return false;

		int BufferSize = imageWidth * imageHeight * 3;
		std::unique_ptr<float[]> resImage = std::make_unique<float[]>(BufferSize);

		oidn::DeviceRef Device = oidn::newDevice();
		Device.commit();

		oidn::FilterRef Filter = Device.newFilter("RT");
		Filter.setImage("color", inMap, oidn::Format::Float3, imageWidth, imageHeight);
		Filter.setImage("output", resImage.get(), oidn::Format::Float3, imageWidth, imageHeight);

		if (colorMap)
		{
			Filter.setImage("albedo", colorMap, oidn::Format::Float3, imageWidth, imageHeight);
		}
		if (normalMap)
		{
			Filter.setImage("normal", normalMap, oidn::Format::Float3, imageWidth, imageHeight);
		}

		Filter.commit();

		Filter.execute();

		//outImage is not created in This Function
		for (int i = 0; i < BufferSize; i += 1)
		{
			outImage[i] = static_cast<uint8_t>(resImage[i] * 255.999);
		}
		return true;
	}
};