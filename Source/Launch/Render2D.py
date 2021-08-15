import Render.Scene2D as Scene2D
from Render.Print import Print

def main():
    Print('Render 2D Start')

    Scene2D.Init(1024,768,512)

    Scene2D.Render('D:\\Out.png')
    Scene2D.Destroy()

    Print('Render 2D End')

if __name__=='__main__':
    main()