from . import message
from . import config
import ctypes

try:
    if config.encode_video:
        import numpy as np
        import cv2 as cv
except:
    message.py_print('You should install opencv and numpy.')
    exit()

use_gpu=False

if use_gpu:
    try:
        render_lib=ctypes.cdll.LoadLibrary('./lib/render-gpu.so')
    except:
        message.py_print('Load gpu library failed.')
        message.py_print('Try to load cpu library.')
        use_gpu=False

if not use_gpu:
    try:
        render_lib=ctypes.cdll.LoadLibrary('./lib/render.so')
    except:
        message.py_print('Please build the lib first.')
        exit()
else:
    message.py_print('Load gpu library done.')

class Render:
    def __init__(self):
        self.init_lib=render_lib.py_init_lib
        self.init_lib.argtypes=None
        self.init_lib.restype=None

        self.clear_lib=render_lib.py_clear_lib
        self.clear_lib.argtypes=None
        self.clear_lib.restype=None

        self.set_args=render_lib.py_set_args
        self.set_args.argtypes= \
        (
            ctypes.POINTER(ctypes.c_int), \
            ctypes.POINTER(ctypes.c_double)
        )
        self.set_args.restype=None

        self.render=render_lib.py_generate
        self.render.argtypes=(ctypes.POINTER(ctypes.c_double),)
        self.render.restype=None

    def __del__(self):
        message.py_print('Release python render.')

    def apply_config(self):
        int_args= \
        [
            config.screen_width, \
            config.screen_height, \
            config.screen_fov, \
            config.backgroung_color[0], \
            config.backgroung_color[1], \
            config.backgroung_color[2], \
            config.core_number, \
            config.sample_time, \
            config.encode_image
        ]
        double_args= \
        [
            config.camera_pos[0], \
            config.camera_pos[1], \
            config.camera_pos[2], \
            config.target_pos[0], \
            config.target_pos[1], \
            config.target_pos[2]
        ]

        int_args=[int(i) for i in int_args]
        double_args=[float(i) for i in double_args]
        int_args_number=len(int_args)
        double_args_number=len(double_args)
        int_args=(ctypes.c_int*int_args_number)(*int_args)
        double_args=(ctypes.c_double*double_args_number)(*double_args)

        self.set_args(int_args,double_args)

    def run(self):
        self.init_lib()
        self.apply_config()

        if config.encode_video:
            encode=cv.VideoWriter_fourcc(*'mp4v')
            video_encoder=cv.VideoWriter('./video/Encode.mp4',encode,30, \
                (config.screen_width,config.screen_height),True)

        for frame_index in range(1,config.render_frame+1):
            img_size=int(config.screen_height*config.screen_width*3)
            pixels=(ctypes.c_double*img_size)(*[0 for i in range(img_size)])
            self.render(pixels)

            if config.encode_video:
                frame=np.array([min(255,max(0,int(i*255))) for i in pixels],dtype=np.uint8)
                frame=frame.reshape((config.screen_width,config.screen_height,3))
                frame=cv.cvtColor(frame,cv.COLOR_RGB2BGR)
                video_encoder.write(frame)
        
        if config.encode_video:
            video_encoder.release()

        self.clear_lib()

def main():
    message.py_print('Please dont run this script.')

if __name__=='__main__':
    main()
