#version 330

in vec3 vPos;
in vec2 vUV;
in vec3 vNorm;

out vec4 outColour;
out vec4 outNormal;
out vec4 outPosition;

uniform sampler2D textureSampler;

void main()
{
    vec4 colour = texture(textureSampler, vUV);
    outColour = colour;
    outNormal = vec4(vNorm,1);
    outPosition = vec4(vPos,1);
}
