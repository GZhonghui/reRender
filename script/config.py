from . import message

#渲染算法
render_types= \
    {
        'direct_light':1,
        'path_tracing':2,
        'bpath_tracing':3,
        'photon_mapping':4
    }

#工程名
ProjectName='Ray Tracing'

#使用的渲染方法
render_type='path_tracing'

#渲染帧数
render_frame=1

#分辨率
screen_width=1024
screen_height=1024

#视野角度
screen_fov=90

#摄像机位置
camera_pos=[0,-5,0]

#摄像机的目标点
target_pos=[0,0,0]

#背景色
backgroung_color=[19,111,152]

#核心数
core_number=32

#采样次数
sample_time=8

#是否启用天空盒
enable_skybox=True

#是否用C语言编码图像
encode_image=False

#是否使用OpenCV编码视频
encode_video=True

#发射光线是否进行扰动
random_light=False

def main():
    message.py_print('Please dont run this script.')

if __name__=='__main__':
    main()
