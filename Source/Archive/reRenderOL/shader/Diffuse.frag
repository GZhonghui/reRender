#version 330 core

out vec4 FragColor;

uniform vec3 DiffuseColor;

void main()
{   
    FragColor = vec4(DiffuseColor, 1.0);
}