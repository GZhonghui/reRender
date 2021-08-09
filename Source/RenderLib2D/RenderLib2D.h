// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the RENDERLIB2D_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// RENDERLIB2D_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef RENDERLIB2D_EXPORTS
#define RENDERLIB2D_API __declspec(dllexport)
#else
#define RENDERLIB2D_API __declspec(dllimport)
#endif

// This class is exported from the dll
class RENDERLIB2D_API CRenderLib2D {
public:
	CRenderLib2D(void);
	// TODO: add your methods here.
};

extern RENDERLIB2D_API int nRenderLib2D;

RENDERLIB2D_API int fnRenderLib2D(void);
