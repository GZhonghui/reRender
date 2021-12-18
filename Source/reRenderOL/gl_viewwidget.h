#ifndef GL_VIEWWIDGET_H
#define GL_VIEWWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>

#include <QPixmap>

#include "tool_aio.h"
#include "math_aio.h"

#include "shader.h"
#include "gl_misc.h"

class GL_ViewWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    GL_ViewWidget(QWidget *parent);
    ~GL_ViewWidget() { Destroy(); }

protected:
    // Skybox
    uint32_t m_SkyboxTextureID;
    uint32_t m_SkyboxShaderProgramID;
    uint32_t m_SkyboxVAOID;
    uint32_t m_SkyboxVBOID;

private: // Tools
    void initSkybox();
    void destroySkybox();
    void changeSkybox(int Which, const QPixmap& Image);
    void renderSkybox(glm::mat4* VP);

    void Destroy();

protected:
    void initializeGL();
    void paintGL();
};

#endif // GL_VIEWWIDGET_H
