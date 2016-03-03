#version 330

layout(location=0) in vec2 vertPos;
layout(location=1) in vec2 vertUV;

out vec2 vUV;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
    vUV = vertUV*vec2(1,-1);
    gl_Position = projMat * viewMat * modelMat * vec4(vertPos,0,1);
}
