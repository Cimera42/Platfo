#version 330

in vec3 vPos;
in vec2 vUV;
in vec3 vNorm;

out vec4 outColour;

uniform sampler2D textureSampler;

void main()
{
    vec4 colour = texture(textureSampler, vUV);
    colour.rgb *= clamp(dot(vNorm, normalize(vec3(-0.5,1,-1))),0,1) + 0.1;
    outColour = colour;
}
