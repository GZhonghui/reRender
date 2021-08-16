#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"Scene.h"

void Scene::Render(const char* filePath)
{
    unsigned int bufferSize = m_Width * m_Height * 3;
    unsigned char* renderBuffer = new unsigned char[bufferSize];

    // TBB
    tbb::parallel_for
    (
        tbb::blocked_range<int>(1, m_Height + 1), [&](tbb::blocked_range<int> hIndexs)
        {
            for (int hIndex = hIndexs.begin(); hIndex < hIndexs.end(); ++hIndex)
            {
                for (int wIndex = 1; wIndex <= m_Width; wIndex += 1)
                {
                    Color Result(0, 0, 0);

                    for (auto sIndex = 1; sIndex <= m_Sample; sIndex += 1)
                    {
                        Result += Sample(Point(wIndex, hIndex), sIndex);
                    }

                    Result = Result / m_Sample;

                    unsigned int pixelIndex = ((hIndex - 1) * m_Width + (wIndex - 1)) * 3;

                    renderBuffer[pixelIndex + 0] = static_cast<unsigned char>(Limit(Result.x(), 0, 1) * 255.999);
                    renderBuffer[pixelIndex + 1] = static_cast<unsigned char>(Limit(Result.y(), 0, 1) * 255.999);
                    renderBuffer[pixelIndex + 2] = static_cast<unsigned char>(Limit(Result.z(), 0, 1) * 255.999);
                }
            }
        }
    );

    // Single Core
    /*
    for (auto hIndex = 1; hIndex <= m_Height; hIndex += 1)
    {
        for (auto wIndex = 1; wIndex <= m_Width; wIndex += 1)
        {
            Color Result(0, 0, 0);

            for (auto sIndex = 1; sIndex <= m_Sample; sIndex += 1)
            {
                
                Result += Sample(Point(wIndex, hIndex), sIndex);
            }

            Result = Result / m_Sample;

            unsigned int pixelIndex = ((hIndex - 1) * m_Width + (wIndex - 1)) * 3;

            renderBuffer[pixelIndex + 0] = static_cast<unsigned char>(Limit(Result.x(), 0, 1) * 255.999);
            renderBuffer[pixelIndex + 1] = static_cast<unsigned char>(Limit(Result.y(), 0, 1) * 255.999);
            renderBuffer[pixelIndex + 2] = static_cast<unsigned char>(Limit(Result.z(), 0, 1) * 255.999);
        }
    }
    */

    stbi_write_png(filePath, m_Width, m_Height, 3, renderBuffer, 0);

    delete[] renderBuffer;

    Out::Log(pType::MESSAGE, "Render to %s", filePath);
}