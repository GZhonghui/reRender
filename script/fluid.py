from . import message
from . import config
import ctypes

show_result=True
use_gpu=True

try:
    import cv2 as cv
    import numpy
    import os
except ModuleNotFoundError:
    message.py_print('You havent install OpenCV rightly.')
    message.py_print('The render result wont show or encode to video.')
    show_result=False

try:
    fluid=ctypes.cdll.LoadLibrary('./lib/fluid-gpu.so')
except:
    message.py_print('Load gpu library failed.')
    message.py_print('Try load cpu library.')
    use_gpu=False

if not use_gpu:
    try:
        fluid=ctypes.cdll.LoadLibrary('./lib/fluid.so')
    except:
        message.py_print('Please build the lib first.')
        exit()

class Fluid:
    def __init__(self):
        self.init_lib=fluid.py_init_lib
        self.init_lib.argtypes=None
        self.init_lib.restype=None

        self.clear_lib=fluid.py_clear_lib
        self.clear_lib.argtypes=None
        self.clear_lib.restype=None

        self.set_args=fluid.py_set_args
        self.set_args.argtypes=(ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_double))
        self.set_args.restype=None

        self.render=fluid.py_generate
        self.render.argtypes=(ctypes.POINTER(ctypes.c_double),)
        self.render.restype=None
    
    def apply_config(self):
        int_args= \
        [
            config.screen_width,config.screen_height,config.screen_fov, \
            config.particle_size_x,config.particle_size_y,config.particle_size_z, \
            config.box_size_x,config.box_size_y,config.box_size_z, \
            config.ground_size, \
            config.backgroung_color_r,config.backgroung_color_g,config.backgroung_color_b, \
            config.core_number,config.sample_time, \
            config.time_step
        ]
        double_args= \
        [
            config.particle_radius, \
            config.particle_start_x,config.particle_start_y,config.particle_start_z, \
            config.box_start_x,config.box_start_y,config.box_start_z, \
            config.camera_pos_x,config.camera_pos_y,config.camera_pos_z, \
            config.camera_dir_x,config.camera_dir_y,config.camera_dir_z, \
            config.camera_up_x,config.camera_up_y,config.camera_up_z
        ]

        int_args_number=len(int_args)
        double_args_number=len(double_args)
        int_args=(ctypes.c_int*int_args_number)(*int_args)
        double_args=(ctypes.c_double*double_args_number)(*double_args)

        self.set_args(int_args,double_args)
    
    def run(self):
        self.init_lib()
        self.apply_config()

        if show_result:
            cv.namedWindow(config.ProjectName,cv.WINDOW_AUTOSIZE)
        for frame_index in range(config.render_frame):
            img_size=int(config.screen_height*config.screen_width*3)
            pixels=(ctypes.c_double*img_size)(*[0 for i in range(img_size)])
            self.render(pixels)
            if not show_result:
                continue
            render_img=numpy.zeros((config.screen_width,config.screen_height,3))
            pixels=[int(i*255) for i in pixels]
            pixel_index=int(0)
            for height_index in range(config.screen_height):
                for width_index in range(config.screen_width):
                    render_img[width_index,height_index,:]=pixels[pixel_index:pixel_index+3]
                    pixel_index+=3
            cv.imshow(config.ProjectName,render_img)
        self.clear_lib()
        if show_result:
            cv.destroyAllWindows()

        if show_result:
            message.py_print('Start encode video.')
            
            imgs=sorted(os.listdir('./render'))
            render_video=cv.VideoWriter('./video/Fluid.mp4',cv.VideoWriter_fourcc('m','p','4','v'), \
                int(1000/config.time_step),(config.screen_width,config.screen_height))
            for render_img in imgs:
                if render_img.split('.')[-1]=='png':
                    frame=cv.imread('./render/'+render_img)
                    render_video.write(frame)
            render_video.release()

            message.py_print('Encode video done.')

def main():
    message.py_print('Please dont run this script.')

if __name__=='__main__':
    main()