from . import message

#渲染算法
render_types= \
{
    'direct_light':1,
    'path_tracing':2,
    'photon_mapping':3,
    'bidirectional_path_tracing':4,
    'metropolis_light_transport':5
}

#工程名 不传递
ProjectName='Ray Tracing'

#使用的渲染方法 作为整数传递
render_type='direct_light'

#是否使用CUDA 不传递
use_gpu=False

#渲染帧数 不传递
render_frame=3

#分辨率 作为整数传递
screen_width=1920
screen_height=1080

#视野角度 作为整数传递
screen_fov=90

#摄像机位置 作为浮点数传递
camera_pos=[0,-5,0]

#摄像机的目标点 作为浮点数传递
target_pos=[0,0,0]

#背景色 作为整数传递
backgroung_color=[19,111,152]

#是否启用天空盒 作为整数传递
enable_skybox=True

#是否用C语言编码图像 作为整数传递
encode_image=True

#是否使用OpenCV编码视频 不传递
encode_video=True

#发射光线是否进行扰动 作为整数传递
random_light=False

#核心数 作为整数传递
core_number=32

#采样次数 作为整数传递
sample_time=8

if __name__=='__main__':
    message.main()
