#version 330

layout(location=0) in vec2 vertPos;
layout(location=1) in vec2 vertUV;

out vec2 vUV;

void main()
{
    vUV = vertUV;
    gl_Position = vec4(vertPos,0,1);
}
