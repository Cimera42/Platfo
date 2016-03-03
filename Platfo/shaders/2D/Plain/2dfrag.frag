#version 330

in vec2 vUV;

out vec4 outColour;

uniform sampler2D textureSampler1;

void main()
{
    outColour = texture(textureSampler1, vUV);
}
