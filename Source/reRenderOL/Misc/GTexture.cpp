#include "GTexture.h"

#include <STB/stb_image.h>
#include <STB/stb_image_write.h>

bool Texture::loadFrom(const char* filePath)
{
	auto read_res = stbi_load(filePath, &width, &height, &channal, 3);
	if (!read_res)
	{
		return false;
	}

	if (m_Pixels)
	{
		m_Pixels.release();
	}

	int image_size = width * height * 3;
	m_Pixels = std::make_unique<uint8_t[]>(image_size);

	memcpy(m_Pixels.get(), read_res, image_size);

	stbi_image_free(read_res);

	return true;
}

Color Texture::Sample(TextureCoords coords) const
{
	return Sample(coords.x(), coords.y());
}


Color Texture::Sample(double u, double v) const
{
	if (!m_Pixels) return colorBlack;

	const bool RepeatMode = true;

	if (RepeatMode)
	{
		//RepeatMode
		u = u - floor(u);
		v = v - floor(v);
	}
	else
	{
		//Clamp Mode
		u = Clamp(u, 0, 1);
		v = Clamp(v, 0, 1);
	}

	int widthIndex = std::min(width, (int)(u * width) + 1);
	int heightIndex = std::max(1, height - (int)(v * height));
	int pixelID = 3 * (width * (heightIndex - 1) + widthIndex - 1);

	return Color(m_Pixels[pixelID + 0], m_Pixels[pixelID + 1], m_Pixels[pixelID + 2]) / 255.999;
}