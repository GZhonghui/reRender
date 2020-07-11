from config import *
import ctypes

try:
    import PIL
except:
    print('Please install pillow first.')
    exit()

try:
    fluid=ctypes.cdll.LoadLibrary('./lib/fluid.so')
except:
    print('Please build the lib first.')
    exit()

render_path='./render/'

class Fluid:
    def __init__(self):
        self.init_lib=fluid.py_init_lib
        self.init_lib.argtypes=None
        self.init_lib.restype=None

        self.clear_lib=fluid.py_clear_lib
        self.clear_lib.argtypes=None
        self.clear_lib.restype=None

        args_group=tuple([ctypes.c_int for i in range(31)])
        self.set_args=fluid.py_set_args
        self.set_args.argtypes=args_group
        self.set_args.restype=None

        self.render=fluid.py_generate
        self.render.argtypes=(ctypes.POINTER(ctypes.c_double))
        self.render.restype=None
    
    def apply_config(self):
        self.set_args(screen_width,screen_height,screen_fov, \
            particle_size_x,particle_size_y,particle_size_z, \
            particle_start_x,particle_start_y,particle_start_z)
    
    def run(self):
        self.init_lib()
        self.apply_config()
        while True:
            img_size=int(screen_height*screen_width*3)
            pixels=(ctypes.c_double*img_size)(*[0 for i in range(img_size)])
            self.render(pixels)
            img=PIL.Image.new('RGB',(screen_width,screen_height))
            pixel_index=int(0)
            pixels=[int(i*255) for i in pixels]
            for height_index in range(screen_height):
                for width_index in range(screen_width):
                    img.putpixel((width_index,height_index), \
                        (pixels[pixel_index],pixels[pixel_index+1],pixels[pixel_index+2]))
            img.show()
        self.clear_lib()

def main():
    print('Please dont run this script.')

if __name__=='__main__':
    main()