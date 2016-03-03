#version 330
#define MAX_DIRECTIONAL_LIGHTS 3
#define MAX_POINT_LIGHTS 5

in vec2 vUV;

out vec4 outColour;

uniform sampler2D textureSampler1; //tex
uniform sampler2D textureSampler2; //norm
uniform sampler2D textureSampler3; //pos
uniform sampler2D textureSampler4; //depth

uniform vec3  directionalLight_direction[MAX_DIRECTIONAL_LIGHTS];
uniform float directionalLight_intensity[MAX_DIRECTIONAL_LIGHTS];
uniform vec3  directionalLight_colour[MAX_DIRECTIONAL_LIGHTS];

uniform vec3  pointLight_location[MAX_POINT_LIGHTS];
uniform float pointLight_intensity[MAX_POINT_LIGHTS];
uniform float pointLight_attenuation[MAX_POINT_LIGHTS];
uniform vec3  pointLight_colour[MAX_POINT_LIGHTS];

vec3 findDirectionalLight(int lightNumber, vec3 fragmentNormal)
{
    float coefficient = dot(fragmentNormal, normalize(directionalLight_direction[lightNumber]));
    float unNegd = max(0, coefficient);
    float totalLightIntensity = unNegd * directionalLight_intensity[lightNumber];

    return directionalLight_colour[lightNumber] * totalLightIntensity;
}

vec3 calculateAllDirectionalLights(vec3 fragmentNormal)
{
    vec3 total = vec3(0,0,0);
    for(int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++)
    {
        total += findDirectionalLight(i, fragmentNormal);
    }
    return total;
}

vec3 findPointLight(int lightNumber, vec3 fragmentPos, vec3 fragmentNormal)
{
    vec3 lightDirection = pointLight_location[lightNumber] - fragmentPos;
    vec3 normedLightDirection = normalize(lightDirection);

    float coefficient = dot(fragmentNormal, normedLightDirection);
    float unNegd = max(0, coefficient);

    float distanceToLight = length(lightDirection);
    float attenuation = clamp(1.0 - (distanceToLight*distanceToLight)/(pointLight_attenuation[lightNumber]*pointLight_attenuation[lightNumber]), 0.0, 1.0);
    attenuation *= attenuation;

    float totalLightIntensity = unNegd * pointLight_intensity[lightNumber] * attenuation;

    return pointLight_colour[lightNumber] * totalLightIntensity;
}

vec3 calculateAllPointLights(vec3 fragmentPos, vec3 fragmentNormal)
{
    vec3 total = vec3(0,0,0);
    for(int i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        total += findPointLight(i, fragmentPos, fragmentNormal);
    }
    return total;
}

void main()
{
    vec4 tex =  texture(textureSampler1, vUV);
    vec3 norm = texture(textureSampler2, vUV).rgb;
    vec3 pos =  texture(textureSampler3, vUV).rgb;

    vec3 lighting = vec3(0,0,0);
    lighting += calculateAllDirectionalLights(norm);
    lighting += calculateAllPointLights(pos, norm);
    outColour = vec4(tex.rgb*lighting,tex.a);
}
