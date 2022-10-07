#ifndef GL_MISC_H
#define GL_MISC_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>

#include <STB/stb_image.h>
#include <STB/stb_image_write.h>

#include "ToolAIO.h"
#include "MathAIO.h"

#include "Loader.h"
#include "Shader.h"

namespace GlMisc
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
            Out::Log(pType::WARNING, "Shader Compile Failed : %s", compileInfo);
        }

        return ShaderID;
    }

    inline unsigned int GenDefaultTextureWithImageFile(QOpenGLFunctions_3_3_Core* f, const char* texturePath)
    {
        unsigned int textureID = 0;

        f->glGenTextures(1, &textureID);
        f->glBindTexture(GL_TEXTURE_2D, textureID);

        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int textureWidth, textureHeight, textureChannels;
        unsigned char* TextureData = stbi_load(texturePath, &textureWidth, &textureHeight, &textureChannels, 3);

        if (!TextureData)
        {
            Out::Log(pType::CRASH, "Generate Texture with Image Failed : %s", texturePath);
            f->glDeleteTextures(1, &textureID);

            return 0;
        }

        f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);
        f->glGenerateMipmap(GL_TEXTURE_2D);

        f->glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(TextureData);

        return textureID;
    }
}

#endif // GL_MISC_H