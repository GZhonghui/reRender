#version 330 core

layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aNormal;
// layout (location = 2) in vec2 aUV;
// layout (location = 3) in vec3 aTangent;
// layout (location = 4) in vec3 aBitangent;

// uniform mat4 M;
// uniform mat4 V;
// uniform mat4 P;

void main()
{
    // gl_Position = P * V * M * vec4(aPos, 1.0);
    gl_Position = vec4(aPos, 1.0);
}