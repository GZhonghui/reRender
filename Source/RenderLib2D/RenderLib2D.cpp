// RenderLib2D.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "RenderLib2D.h"


// This is an example of an exported variable
RENDERLIB2D_API int nRenderLib2D=0;

// This is an example of an exported function.
RENDERLIB2D_API int fnRenderLib2D(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
CRenderLib2D::CRenderLib2D()
{
    return;
}
