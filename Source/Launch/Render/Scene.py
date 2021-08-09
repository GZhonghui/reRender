# Python Virtual Env
# OpenCV
# NumPy
# reprint

from Render.Print import Print

try:
    import cv2 as cv
    import numpy as np

    from reprint import output

    from Render.Print import GetBar

    import struct

    from os import path

    import datetime

    import ctypes
    from ctypes import c_int,c_float,c_char_p

except ImportError:
    Print('Missing Python Packages')
    exit(0)

try:
    render_lib=ctypes.cdll.LoadLibrary('./Render/Libs/RenderLib.dll')

except Exception as message:
    Print('Cant Load the Render Lib')
    Print(str(message))
    exit(0)

else:
    Print('Loading the Render Lib Function')

    #Test Function
    LibSay=render_lib.LibSay
    LibSay.argtypes=None
    LibSay.restype=None

    #Init The Total Lib
    initAllLib=render_lib.initAllLib
    initAllLib.argtypes=None
    initAllLib.restype=None

    #Run There Only Once
    initAllLib()

    CreateAScene=render_lib.CreateAScene
    CreateAScene.argtypes=None
    CreateAScene.restype=c_int

    LoadAModel=render_lib.LoadAModel
    LoadAModel.argtypes=(c_int,c_char_p,c_int)
    LoadAModel.restype=c_int

    LoadSky=render_lib.LoadSky
    LoadSky.argtypes=(c_int,c_char_p)
    LoadSky.restype=c_int

    buildBVH=render_lib.buildBVH
    buildBVH.argtypes=(c_int,)
    buildBVH.restype=c_int

    applyConfig=render_lib.applyConfig
    applyConfig.argtypes=(c_int,c_char_p)
    applyConfig.restype=c_int

    Render=render_lib.Render
    Render.argtypes=(c_int,c_char_p)
    Render.restype=c_int

    RenderDone=render_lib.RenderDone
    RenderDone.argtypes=(c_int,)
    RenderDone.restype=c_int

    UpdateRenderProgress=render_lib.UpdateRenderProgress
    UpdateRenderProgress.argtypes=(c_int,c_char_p)
    UpdateRenderProgress.restype=c_int

    deNoise=render_lib.deNoise
    deNoise.argtypes=(c_int,c_char_p,c_char_p)
    deNoise.restype=c_int

class StaticScene:
    
    def __init__(self):
        #Render View
        self.shouldCloseWindow=False
        self.SceneReady=False

        self.sceneID=CreateAScene()

        self.autoCloseWindow=False
        self.lessOutput=True

        #Default Render Config
        self.SetRenderConfig('spp',2)
        self.SetRenderConfig('core',2)
        self.SetRenderConfig('resolution',(1024,768))
        self.SetRenderConfig('horizontal_angle',120)
        self.SetRenderConfig('camera_pos',(0,0,0))
        self.SetRenderConfig('target_pos',(100,0,0))
        self.SetRenderConfig('encode_path','./Render.png')
        self.SetRenderConfig('enableMSAA',0)
        self.SetRenderConfig('enableGammaCorrection',0)

    def __del__(self):
        pass

    def LoadFile(self,filePath:str,fileType:str):

        #File Path
        filePath=path.abspath(filePath)

        if path.isfile(filePath):
            #c_char_p(str.encode()) -> char*
            if fileType=='Self':
                LoadAModel(c_int(self.sceneID),c_char_p(filePath.encode()),c_int(1))
                #Scene has been changed
                self.SceneReady=False

            elif fileType=='Standard':
                LoadAModel(c_int(self.sceneID),c_char_p(filePath.encode()),c_int(2))
                self.SceneReady=False

            else:
                Print('Unknown File Type')

        elif path.isdir(filePath):
            if fileType=='Skybox':
                LoadSky(c_int(self.sceneID),c_char_p(filePath.encode()))

        else:
            Print('Wrong Path')

    def SetRenderConfig(self,which:str,value):
        try:
            if which=='ssp':
                self.ssp=int(value)
            elif which=='core':
                self.core=int(value)
            elif which=='resolution':
                self.resolution=[int(value[i]) for i in range(2)]
            elif which=='horizontal_angle':
                self.horizontal_angle=int(value)
            elif which=='camera_pos':
                self.camera_pos=[float(value[i]) for i in range(3)]
            elif which=='target_pos':
                self.target_pos=[float(value[i]) for i in range(3)]
            elif which=='encode_path':
                self.encode_path=path.abspath(str(value))
            elif which=='enableMSAA':
                self.enableMSAA=int(value)
            elif which=='enableGammaCorrection':
                self.enableGammaCorrection=int(value)
            else:
                Print('Unknown Config Type')
        except Exception as Message:
            Print('Wrong Args:',Message)

    def ApplyRenderConfig(self):

        #Total 
        ArgBytes=bytes()

        ArgBytes+=self.ssp.to_bytes(4,byteorder='little')
        ArgBytes+=self.core.to_bytes(4,byteorder='little')
        ArgBytes+=self.resolution[0].to_bytes(4,byteorder='little')
        ArgBytes+=self.resolution[1].to_bytes(4,byteorder='little')
        ArgBytes+=self.horizontal_angle.to_bytes(4,byteorder='little')

        #Float Default 4 Bytes
        ArgBytes+=struct.pack('f',self.camera_pos[0])
        ArgBytes+=struct.pack('f',self.camera_pos[1])
        ArgBytes+=struct.pack('f',self.camera_pos[2])
        ArgBytes+=struct.pack('f',self.target_pos[0])
        ArgBytes+=struct.pack('f',self.target_pos[1])
        ArgBytes+=struct.pack('f',self.target_pos[2])

        #2 Bytes
        ArgBytes+=self.enableMSAA.to_bytes(2,byteorder='little')
        ArgBytes+=self.enableGammaCorrection.to_bytes(2,byteorder='little')

        applyConfig(c_int(self.sceneID),c_char_p(ArgBytes))

    def RebuildScene(self):
        buildBVH(c_int(self.sceneID))
        self.SceneReady=True

    #Wait to Render Done
    def Render(self):

        self.ApplyRenderConfig()

        if not self.SceneReady:
            Print('Scene Need to Rebuild')
            self.RebuildScene()

        FrameShape=(self.resolution[1],self.resolution[0],3)
        Frame=np.zeros(shape=FrameShape,dtype=np.uint8)

        #Let the Lib Fill
        RenderBufferSize=self.resolution[0]*self.resolution[1]*3
        RenderBuffer=bytes(RenderBufferSize)

        #Launch the Render thread
        Render(c_int(self.sceneID),c_char_p(RenderBuffer))

        RenderStartTime=datetime.datetime.now()

        SortKey=dict()
        SortKey['[ Total ]']='1' #First
        SortKey['[ Spend ]']='3' #Middle
        SortKey['[Waiting]']='4' #Last
        def SortList(key):
            if key[0] in SortKey.keys():
                return SortKey[key[0]]
            else:
                return '2'+str(key[0]) #Middle

        BarWidth=60
        #DO NOT OUTPUT ANYTHING During Rendering
        with output(output_type='dict',sort_key=SortList) as RenderProcress:
            RenderProcress['[ Total ]']=GetBar(0,self.core*self.ssp,BarWidth)
            RenderProcress['[Waiting]']='Starting'
            RenderProcress['[ Spend ]']='Starting'
            if not self.lessOutput:
                for i in range(self.core):
                    RenderProcress['[Core%03d]'%(i+1)]=GetBar(0,100,BarWidth)

            #Remember to Set
            self.shouldCloseWindow=False
            while not self.shouldCloseWindow:
                ProcressBuffer=bytes((self.core+1)*4)
                #Get Now Render Progress
                if int(UpdateRenderProgress(c_int(self.sceneID),ProcressBuffer)):
                    Frame=np.frombuffer(RenderBuffer,dtype=np.uint8)
                    Frame=Frame.reshape(FrameShape)

                    #Update Now State to Window
                    cv.imshow('RenderView',Frame)

                Total=int.from_bytes(ProcressBuffer[0:4],byteorder='little')
                for i in range(self.core):
                    ThisBar=int.from_bytes(ProcressBuffer[(i+1)*4:(i+2)*4],byteorder='little')
                    if not self.lessOutput:
                        RenderProcress['[Core%03d]'%(i+1)]=GetBar(ThisBar,100,BarWidth)
                    Total+=ThisBar/101#Littele Tick
                RenderProcress['[ Total ]']=GetBar(Total,self.core*self.ssp,BarWidth)
                if Total>0:
                    NowTime=datetime.datetime.now()
                    FullTotal=self.core*self.ssp
                    RemainWaitingTime=(NowTime-RenderStartTime)*(max(0,FullTotal-Total)/Total)
                    if RemainWaitingTime.seconds<=3:
                        RenderProcress['[Waiting]']='Shutting'
                    else:
                        RenderProcress['[Waiting]']=str(RemainWaitingTime)
                    RenderProcress['[ Spend ]']=str(NowTime-RenderStartTime)

                Key=cv.waitKey(100)

                if int(RenderDone(c_int(self.sceneID))):
                    self.shouldCloseWindow=True

                    #Fill Bar
                    RenderProcress['[ Total ]']=GetBar(100,100,BarWidth)
                    RenderProcress['[Waiting]']='Render Done'
                    RenderProcress['[ Spend ]']='Render Done'
                    if not self.lessOutput:
                        for i in range(self.core):
                            RenderProcress['[Core%03d]'%(i+1)]=GetBar(100,100,BarWidth)

        Print('Render Done')
        RenderEndTime=datetime.datetime.now()
        SpendTime=RenderEndTime-RenderStartTime
        Print('Spend Time='+str(SpendTime))

        #After Render Done Get the Final Frame
        Frame=np.frombuffer(RenderBuffer,dtype=np.uint8)
        Frame=Frame.reshape(FrameShape)

        #Save the Final Frame
        cv.imwrite(self.encode_path,Frame)
        Print('Origin Render Result Saved to '+self.encode_path)

        #dNoise
        Print('dNoiseing')
        #OpenCV deNoise
        #dNoise=cv.fastNlMeansDenoisingColored(Frame,None,12,10,7,21)
        
        #Intel deNoise
        deNoiseBuffer=bytes(RenderBufferSize)
        deNoise(self.sceneID,RenderBuffer,deNoiseBuffer)
        
        #Show deNoise Result
        Frame=np.frombuffer(deNoiseBuffer,dtype=np.uint8)
        Frame=Frame.reshape(FrameShape)
        deNoisePath=path.join(path.dirname(self.encode_path),'deNoise.png')
        cv.imwrite(deNoisePath,Frame)
        cv.imshow('RenderView',Frame)

        Print('dNoise Done')

        if not self.autoCloseWindow:
            while True:
                Key=cv.waitKey(100)
                #Window Closed
                if cv.getWindowProperty('RenderView',0)<0:
                    break
        cv.destroyAllWindows()