#version 330

in vec3 outUV;
in vec3 outPos;

out vec4 outColour;
out vec4 outNormal;
out vec4 outPosition;

uniform mat4 viewMat;
uniform mat4 projMat;

uniform samplerCube skyboxTexture;

void main()
{
    vec4 colour = texture(skyboxTexture, outUV);
    outColour = vec4(colour);
    //outNormal = vec4((outPos/2)+0.5,1);
    //outPosition = vec4(1,1,1,1);
}
