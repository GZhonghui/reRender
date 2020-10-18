from . import message

#渲染算法
render_types= \
{
    #直接着色
    #此算法所依赖的参数表如下
    'direct_light':1,

    #路径追踪
    #此算法所依赖的参数表如下
    #核心数目
    #采样次数
    'path_tracing':2,

    #光子映射
    #此算法所依赖的参数表如下
    'photon_mapping':3,

    #双向路径追踪
    #此算法所依赖的参数表如下
    'bidirectional_path_tracing':4,

    #梅特波利斯光照传输
    #此算法所依赖的参数表如下
    'metropolis_light_transport':5
}

#-----Python参数-----
#工程名
ProjectName='Ray Tracing'

#是否使用CUDA
use_gpu=False

#渲染帧数
render_frame=3

#是否使用OpenCV编码视频
encode_video=True

#-----C++全局参数-----
#使用的渲染方法 整数
render_type='path_tracing'

#分辨率 整数
screen_width=1920
screen_height=1080

#视野角度 整数
screen_fov=90

#摄像机位置 浮点数
camera_pos=[0,-5,0]

#摄像机的目标点 浮点数
target_pos=[0,0,0]

#背景色 整数
backgroung_color=[19,111,152]

#是否启用天空盒 整数
enable_skybox=True

#是否用C语言编码图像 整数
encode_image=True

#发射光线是否进行扰动 整数
random_light=False

#-----算法相关参数-----
#核心数 整数
core_number=18

#采样次数 整数
sample_time=2

if __name__=='__main__':
    message.main()
