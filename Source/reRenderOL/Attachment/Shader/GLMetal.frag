#version 330 core

in vec3 thisPosition;
in vec3 thisNormal;

out vec4 FragColor;

uniform vec3 cameraPos;

uniform samplerCube skyboxTexture;

void main()
{   
    vec3 I = normalize(thisPosition - cameraPos);
    vec3 R = reflect(I, normalize(thisNormal));

    FragColor = vec4(texture(skyboxTexture, R).rgb, 1.0);
}