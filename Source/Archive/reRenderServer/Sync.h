#ifndef SYNC_H
#define SYNC_H

#include "ToolAIO.h"
#include "MathAIO.h"

namespace Sync
{
    struct Header
    {
        int64_t dataSize;
        int64_t meshCnt;
        int64_t sphereCnt;

        int64_t renderWidth;
        int64_t renderHeight;
        int64_t renderSPP;

        int64_t coreCnt;

        int64_t workStart;
        int64_t workEnd;

        void Clear()
        {
            dataSize = meshCnt = sphereCnt = 0;
        }
    };

    struct TriangleData
    {
        Point Location[3];
        Direction Normal[3];
        UVCoord UV[3];
    };

    // Material Types
    // 1: Diffuse
    // 2: Metal
    // 3: Glass
    // 4: Emit
    // Material Types

    struct MeshData
    {
        int64_t triangleCnt;
        std::vector<TriangleData> Data;

        int16_t materialType;
        int64_t materialSize;
        QByteArray materialData;
    };

    struct SphereData
    {
        Point Center;
        double Radius;

        int16_t materialType;
        int64_t materialSize;
        QByteArray materialData;
    };

    struct RenderImage
    {
        int64_t imageSize;
        QByteArray imageData;
    };
}

#endif // SYNC_H