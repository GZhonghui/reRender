from . import message
import math

#工程名 可修改
ProjectName='Fluid'

#渲染帧数 可修改
render_frame=int(10)

#摄像机的目标点
target_pos=[250,10,250]

#屏幕参数 分辨率和视野角度 可修改
screen_width=int(1024)
screen_height=int(768)
screen_fov=int(45)

#起始粒子范围 可修改
particle_size_x=int(301)
particle_size_y=int(301)
particle_size_z=int(301)

#边缘碰撞体大小 可修改
box_size_x=int(500)
box_size_y=int(900)
box_size_z=int(500)

#渲染地面大小 可修改
ground_size=int(500)

#背景色 可修改
backgroung_color_r=int(185)
backgroung_color_g=int(227)
backgroung_color_b=int(217)

#核心数和采样次数 可修改
core_number=int(28)
sample_time=int(12)

#时间微分 可修改
time_step=int(20)

#粒子半径 可修改
particle_radius=float(0.5)

#粒子的起始坐标 可修改
particle_start_x=float(100)
particle_start_y=float(500)
particle_start_z=float(100)

#边缘碰撞体的起始坐标 可修改
box_start_x=float(0)
box_start_y=float(0)
box_start_z=float(0)

#摄像机位置 可修改
camera_pos_x=float(700)
camera_pos_y=float(500)
camera_pos_z=float(-200)

#摄像机的方向 请勿修改
camera_dir_x=float(target_pos[0]-camera_pos_x)
camera_dir_y=float(target_pos[1]-camera_pos_y)
camera_dir_z=float(target_pos[2]-camera_pos_z)

#摄像机的上向量 请勿修改
up_y=float((camera_dir_x*camera_dir_x+camera_dir_z*camera_dir_z)/(-camera_dir_y))
camera_up_x=float(camera_dir_x)
camera_up_z=float(camera_dir_z)
camera_up_y=float(up_y)
if camera_up_y<float(0):
    camera_up_x=-camera_up_x
    camera_up_y=-camera_up_y
    camera_up_z=-camera_up_z

def main():
    message.py_print('Please dont run this script.')

if __name__=='__main__':
    main()