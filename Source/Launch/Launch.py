#Launch Port

from Render.Scene import StaticScene

def main():
    
    scene=StaticScene()

    scene.SetRenderConfig('ssp',2)
    scene.SetRenderConfig('core',24)
    scene.SetRenderConfig('target_pos',(0,0,0))
    scene.SetRenderConfig('camera_pos',(10,-10,4))
    scene.SetRenderConfig('horizontal_angle',100)
    scene.SetRenderConfig('resolution',(1024,768))
    scene.SetRenderConfig('enableGammaCorrection',1)
    scene.SetRenderConfig('enableMSAA',1)

    scene.LoadFile('../../Asset/Sphere/Sphere.obj','Self')
    scene.LoadFile('../../Asset/Plane/Plane.obj','Standard')
    scene.LoadFile('../../Asset/Creek','Skybox')

    scene.SetRenderConfig('encode_path','../../Render.png')

    scene.Render()

    return None

if __name__=='__main__':
    main()