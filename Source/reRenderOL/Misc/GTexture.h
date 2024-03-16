#pragma once

#include <memory>
#include <cstdint>

#include "MathAIO.h"
#include "ToolAIO.h"

class Texture
{
public:
	Texture() = default;
	Texture(const char* filePath)
	{
		loadFrom(filePath);
	}

	~Texture() = default;

public:
	bool loadFrom(const char* filePath);
	Color Sample(TextureCoords coords) const;
	Color Sample(double u, double v) const;

protected:
	std::unique_ptr<uint8_t[]> m_Pixels;
	int width, height, channal;
};