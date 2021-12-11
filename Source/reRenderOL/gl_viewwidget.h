#ifndef GL_VIEWWIDGET_H
#define GL_VIEWWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class GL_ViewWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    GL_ViewWidget(QWidget *parent);

protected:
    void initializeGL();
    void paintGL();
};

#endif // GL_VIEWWIDGET_H
