# reRender
![reRender](reRender.png)
---
> More Projects from the reFamily [**reSimulate**](https://github.com/GZhonghui/reSimulate)/[**reEngine**](https://github.com/GZhonghui/reEngine)

## について
**reRender**はオフラインのレンダリングエンジンです

| レイトレーシング |
:-:
| ![3D Model](Res_01_Model.png) |
| AIノイズ除去 |
| ![Denoise](Res_02_deNoise.png) |
| 2Dシーン |
| ![2D Scene](Res_03_2D.png) |

## 解説
**reRender**はPythonパッケージです
```python
from Render.Scene import StaticScene

def main():
    
    scene=StaticScene()

    scene.SetRenderConfig('spp',2)
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
```

## 特徴
### ✔レイトレーシングによる高品質の結果
### ✔BVH加速
### ✔マルチスレッド加速
### ✔複数の素材
### ✔3Dモデルのインポート
### ✔テクスチャでレンダリング
### ✔スカイボックスとアンビエントライトをカスタマイズする
### ✔AIノイズ除去
### ✔アニメーションをレンダリングする
### ✔画像またはビデオにエンコードする
### ✔Signed Distance Field
### ✔2Dシーンのサポート
### ✔リアルタイムの結果プレビュー
### ✔Pythonインターフェースとして使いやすい

## クレジット
> このプロジェクトは以下のライブラリに依存しています
* Assimp
* Boost
* Eigen
* Intel OIDN
* Intel TBB
* NumPy
* OpenCV
* STB
