#version 330
#define MAX_DIRECTIONAL_LIGHTS 3
#define MAX_POINT_LIGHTS 5
#define MAX_SPOT_LIGHTS 5

in vec2 vUV;

out vec4 outColour;

uniform sampler2D textureSampler1; //tex
uniform sampler2D textureSampler2; //norm
uniform sampler2D textureSampler3; //pos
uniform sampler2D textureSampler4; //depth

uniform int   directionalLight_count;
uniform vec3  directionalLight_direction[MAX_DIRECTIONAL_LIGHTS];
uniform float directionalLight_intensity[MAX_DIRECTIONAL_LIGHTS];
uniform vec3  directionalLight_colour[MAX_DIRECTIONAL_LIGHTS];

uniform int   pointLight_count;
uniform vec3  pointLight_location[MAX_POINT_LIGHTS];
uniform float pointLight_intensity[MAX_POINT_LIGHTS];
uniform float pointLight_attenuation[MAX_POINT_LIGHTS];
uniform vec3  pointLight_colour[MAX_POINT_LIGHTS];

uniform int   spotLight_count;
uniform vec3  spotLight_location[MAX_SPOT_LIGHTS];
uniform vec3  spotLight_direction[MAX_SPOT_LIGHTS];
uniform float spotLight_intensity[MAX_SPOT_LIGHTS];
uniform float spotLight_attenuation[MAX_SPOT_LIGHTS];
uniform vec2  spotLight_angle[MAX_SPOT_LIGHTS];
uniform vec3  spotLight_colour[MAX_SPOT_LIGHTS];

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
    for(int i = 0; i < directionalLight_count; i++)
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
    for(int i = 0; i < pointLight_count; i++)
    {
        total += findPointLight(i, fragmentPos, fragmentNormal);
    }
    return total;
}

vec3 findSpotLight(int lightNumber, vec3 fragmentPos, vec3 fragmentNormal)
{
    vec3 fullLengthL = spotLight_location[lightNumber] - fragmentPos;
    vec3 L = normalize(fullLengthL);
    vec3 D = normalize(spotLight_direction[lightNumber]);

    float currentAngle = dot(L, D);
    float innerAngle = spotLight_angle[lightNumber].x;
    float outerAngle = spotLight_angle[lightNumber].y;
    float difference = innerAngle - outerAngle;

    float spot = 0.0;
    spot = clamp((currentAngle - outerAngle)/difference, 0.0,1.0);

    float totalLightIntensity = 0.0;
    float lambert = max(0.0, dot(fragmentNormal, L));
    if(lambert > 0.0)
    {
        float distanceToLight = length(fullLengthL);
        float attenuation = clamp(1.0 - (distanceToLight*distanceToLight)/(spotLight_attenuation[lightNumber]*spotLight_attenuation[lightNumber]), 0.0, 1.0);
        attenuation *= attenuation;

        totalLightIntensity += lambert * spot * attenuation * spotLight_intensity[lightNumber];
    }

    return spotLight_colour[lightNumber] * totalLightIntensity;
}

vec3 calculateAllSpotLights(vec3 fragmentPos, vec3 fragmentNormal)
{
    vec3 total = vec3(0,0,0);
    for(int i = 0; i < spotLight_count; i++)
    {
        total += findSpotLight(i, fragmentPos, fragmentNormal);
    }
    return total;
}

void main()
{
    vec4 tex =  texture(textureSampler1, vUV);
    vec3 norm = texture(textureSampler2, vUV).rgb;
    vec3 pos =  texture(textureSampler3, vUV).rgb;
    float depth = texture(textureSampler4, vUV).r;
    float f = 100.0;
    float n = 0.01;
    depth = (2 * n) / (f + n - depth * (f - n));

    vec3 lighting = vec3(0,0,0);
    lighting += calculateAllDirectionalLights(norm);
    lighting += calculateAllPointLights(pos, norm);
    if(depth >= 0.999)
    {
        outColour = vec4(tex.rgb,tex.a);
    }
    else
    {
        outColour = vec4(tex.rgb*lighting,tex.a);
    }}
