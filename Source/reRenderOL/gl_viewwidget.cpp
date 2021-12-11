#include "gl_viewwidget.h"

GL_ViewWidget::GL_ViewWidget(QWidget *parent):
    QOpenGLWidget(parent)
{

}

void GL_ViewWidget::initializeGL()
{

}

void GL_ViewWidget::paintGL()
{
    QOpenGLFunctions *f = this->context()->functions();
    f->glClearColor(0.0f, 0.2f, 0.0f, 1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}
