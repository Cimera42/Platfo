#version 330
layout(location=0) in vec3 vertPos;

uniform mat4 viewMat;
uniform mat4 projMat;

out vec3 outPos;
out vec3 outUV;

void main()
{
    outUV = vertPos;
    outPos = vertPos;
    gl_Position = projMat * viewMat * (vec4(vertPos, 1.0));
}
