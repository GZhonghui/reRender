#ifndef GL_VIEWWIDGET_H
#define GL_VIEWWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>

#include "shader.h"
#include "gl_misc.h"

class GL_ViewWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    GL_ViewWidget(QWidget *parent);

protected:
    // Skybox
    uint32_t m_SkyboxShaderProgramID;
    uint32_t m_SkyboxVAOID;
    uint32_t m_SkyboxVBOID;

private: // Tools
    void initSkybox();
    void renderSkybox();
    void destroySkybox();

    void Destroy();

protected:
    void initializeGL();
    void paintGL();
};

#endif // GL_VIEWWIDGET_H
