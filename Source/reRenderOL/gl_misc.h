#ifndef GL_MISC_H
#define GL_MISC_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>

#include "MathAIO.h"
#include "ToolAIO.h"

#include "shader.h"

namespace GLMisc
{
    inline unsigned int CompileShader(QOpenGLFunctions_3_3_Core* f, const char* SourceCode, sType Stage)
    {
        unsigned int ShaderID = 0;

        switch (Stage)
        {
        case sType::VERT:
        {
            ShaderID = f->glCreateShader(GL_VERTEX_SHADER);
        }break;

        case sType::FRAG:
        {
            ShaderID = f->glCreateShader(GL_FRAGMENT_SHADER);
        }break;

        default: return 0;
        }

        f->glShaderSource(ShaderID, 1, &SourceCode, nullptr);
        f->glCompileShader(ShaderID);

        int  compileSuccessfully;
        char compileInfo[512];

        f->glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &compileSuccessfully);
        if (!compileSuccessfully)
        {
            f->glGetShaderInfoLog(ShaderID, 512, NULL, compileInfo);
            // Out::Log(pType::WARNING, "Shader Compile Failed : %s", compileInfo);
            // qDebug() << "Shader Compile Failed: " << compileInfo;
        }

        return ShaderID;
    }
}

#endif // GL_MISC_H
