#pragma once

#include "GTexture.h"
#include "MathAIO.h"

class Skybox
{
public:
	Skybox() = default;
	~Skybox() = default;

protected:
	Texture posX;
	Texture negX;
	Texture posY;
	Texture negY;
	Texture posZ;
	Texture negZ;

public:
    //Use '\' Only Work On Windows
	bool LoadFile(const char* fileDir)
	{
		const uint32_t BufferLength = 1024;
		char filePath[BufferLength];

		sprintf_s(filePath, BufferLength, "%s\\posX.jpg", fileDir);
		posX.loadFrom(filePath);

		sprintf_s(filePath, BufferLength, "%s\\negX.jpg", fileDir);
		negX.loadFrom(filePath);

		sprintf_s(filePath, BufferLength, "%s\\posY.jpg", fileDir);
		posY.loadFrom(filePath);

		sprintf_s(filePath, BufferLength, "%s\\negY.jpg", fileDir);
		negY.loadFrom(filePath);

		sprintf_s(filePath, BufferLength, "%s\\posZ.jpg", fileDir);
		posZ.loadFrom(filePath);

		sprintf_s(filePath, BufferLength, "%s\\negZ.jpg", fileDir);
		negZ.loadFrom(filePath);

		return true;
	}

	Color Sample(const Direction& inDir) const
	{
        using std::abs;
		auto Dir = inDir.normalized();
        Color result(1, 0, 0);
        
        double initX = abs(Dir.x()), initY = abs(Dir.y()), initZ = abs(Dir.z());

        if (initX > initY && initX > initZ)
        {
            double t = 1.0 / initX;
            double y = Dir.y() * t, z = Dir.z() * t;
            if (Dir.x() > 0)
            {
                double tx = 1 - (y + 1) / 2.0;
                double ty = (z + 1) / 2.0;
                result = posX.Sample(tx, ty);
            }
            else
            {
                double tx = (y + 1) / 2.0;
                double ty = (z + 1) / 2.0;
                result = negX.Sample(tx, ty);
            }
        }
        else if (initY > initX && initY > initZ)
        {
            double t = 1.0 / initY;
            double x = Dir.x() * t, z = Dir.z() * t;
            if (Dir.y() > 0)
            {
                double tx = (x + 1) / 2.0;
                double ty = (z + 1) / 2.0;
                result = posY.Sample(tx, ty);
            }
            else
            {
                double tx = 1 - (x + 1) / 2.0;
                double ty = (z + 1) / 2.0;
                result = negY.Sample(tx, ty);
            }
        }
        else
        {
            double t = 1.0 / initZ;
            double x = Dir.x() * t, y = Dir.y() * t;
            if (Dir.z() > 0)
            {
                double tx = (x + 1) / 2.0;
                double ty = 1 - (y + 1) / 2.0;
                result = posZ.Sample(tx, ty);
            }
            else
            {
                double tx = (x + 1) / 2.0;
                double ty = (y + 1) / 2.0;
                result = negZ.Sample(tx, ty);
            }
        }
        return result;
	}
};