#version 330

layout(location=0) in vec3 vertPos;
layout(location=1) in vec2 vertUV;
layout(location=2) in vec3 vertNorm;

out vec3 vPos;
out vec2 vUV;
out vec3 vNorm;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
    vPos = vec3(modelMat * vec4(vertPos,1));
    vUV = vertUV*vec2(1,-1);
    mat3 normalMatrix = mat3(modelMat);
    normalMatrix = inverse(normalMatrix);
    normalMatrix = transpose(normalMatrix);
    vNorm = normalize(normalMatrix * vertNorm);
    gl_Position = projMat * viewMat * modelMat * vec4(vertPos,1);
}
