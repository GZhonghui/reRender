from Render.Print import Print

import ctypes
from ctypes import c_int,c_float,c_char_p

try:
    render2d_lib=ctypes.cdll.LoadLibrary('./Render/Libs/RenderLib2D.dll')

except Exception as message:
    Print('Cant Load the Render Lib 2D')
    Print(str(message))
    exit(0)

else:
    Print('Loading the Render Lib 2D Function')

    C_initAllLib=render2d_lib.initAllLib
    C_initAllLib.argtypes=(c_int,c_int,c_int)
    C_initAllLib.restype=None

    C_clearAllLib=render2d_lib.clearAllLib
    C_clearAllLib.argtypes=None
    C_clearAllLib.restype=None

    C_addObject=render2d_lib.addObject
    C_addObject.argtypes=(c_int,)
    C_addObject.restype=None

    C_Render=render2d_lib.Render
    C_Render.argtypes=(c_char_p,)
    C_Render.restype=None

def Init(sceneWidth:int,sceneHeight:int,Sample:int):
    C_initAllLib(c_int(sceneWidth),c_int(sceneHeight),c_int(Sample))

def Destroy():
    C_clearAllLib()

def Add(Type:int):
    # Code Here
    C_addObject(c_int(Type))

def Render(filePath:str):
    C_Render(c_char_p(filePath.encode()))