#version 330

in vec2 vertPos;
in vec2 vertUV;

in mat4 instanceMatrix;

out vec2 vUV;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
    vUV = vertUV*vec2(1,-1);
    gl_Position = projMat * viewMat * modelMat * vec4(vertPos,0,1);
}
