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

uniform vec3 cameraPosition;

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

vec4[2] findDirectionalLight(int lightNumber, vec3 fragmentPos, vec3 fragmentNormal)
{
    vec3 lightDir = normalize(directionalLight_direction[lightNumber]);
    float coefficient = dot(fragmentNormal, lightDir);
    float unNegd = max(0, coefficient);
    float totalLightIntensity = unNegd * directionalLight_intensity[lightNumber];

    vec3 surfaceToCamera = normalize(cameraPosition - fragmentPos);
    vec3 surfaceToLight = lightDir;

    float specularCoefficient = 0.0;
    if(coefficient > 0.0)
    {
        specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, fragmentNormal))), 32);
    }
    float spec = specularCoefficient;

    vec4 returns[2];
    returns[0].xyz = directionalLight_colour[lightNumber] * totalLightIntensity;
    returns[1].xyz = spec * returns[0].xyz;
    returns[1].w = spec * totalLightIntensity;
    return returns;
}

vec4[2] calculateAllDirectionalLights(vec3 fragmentPos, vec3 fragmentNormal)
{
    vec4 total[2];
    for(int i = 0; i < directionalLight_count; i++)
    {
        vec4 vals[2] = findDirectionalLight(i, fragmentPos, fragmentNormal);
        total[0] += vals[0];
        total[1] += vals[1];
    }
    return total;
}

vec4[2] findPointLight(int lightNumber, vec3 fragmentPos, vec3 fragmentNormal)
{
    vec3 lightDirection = pointLight_location[lightNumber] - fragmentPos;
    vec3 normedLightDirection = normalize(lightDirection);

    float coefficient = dot(fragmentNormal, normedLightDirection);
    float unNegd = max(0, coefficient);

    float distanceToLight = length(lightDirection);
    float attenuation = clamp(1.0 - (distanceToLight*distanceToLight)/(pointLight_attenuation[lightNumber]*pointLight_attenuation[lightNumber]), 0.0, 1.0);
    attenuation *= attenuation;

    float totalLightIntensity = unNegd * pointLight_intensity[lightNumber] * attenuation;


    vec3 surfaceToCamera = normalize(cameraPosition - fragmentPos);
    vec3 surfaceToLight = normedLightDirection;

    float specularCoefficient = 0.0;
    if(coefficient > 0.0)
    {
        specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, fragmentNormal))), 32);
    }
    float spec = specularCoefficient;

    vec4 returns[2];
    returns[0].xyz = pointLight_colour[lightNumber] * totalLightIntensity;
    returns[1].xyz = spec * returns[0].xyz;
    returns[1].w = spec * totalLightIntensity;
    return returns;
}

vec4[2] calculateAllPointLights(vec3 fragmentPos, vec3 fragmentNormal)
{
    vec4 total[2];
    for(int i = 0; i < pointLight_count; i++)
    {
        vec4 vals[2] = findPointLight(i, fragmentPos, fragmentNormal);
        total[0] += vals[0];
        total[1] += vals[1];
    }
    return total;
}

vec4[2] findSpotLight(int lightNumber, vec3 fragmentPos, vec3 fragmentNormal)
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
    float specularCoefficient = 0.0;
    if(lambert > 0.0)
    {
        float distanceToLight = length(fullLengthL);
        float attenuation = clamp(1.0 - (distanceToLight*distanceToLight)/(spotLight_attenuation[lightNumber]*spotLight_attenuation[lightNumber]), 0.0, 1.0);
        attenuation *= attenuation;

        totalLightIntensity += lambert * spot * attenuation * spotLight_intensity[lightNumber];

        vec3 surfaceToCamera = normalize(cameraPosition - fragmentPos);
        vec3 surfaceToLight = L;
        specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, fragmentNormal))), 32);
    }
    float spec = specularCoefficient;

    vec4 returns[2];
    returns[0].xyz = spotLight_colour[lightNumber] * totalLightIntensity;
    returns[1].xyz = spec * returns[0].xyz;
    returns[1].w = spec * totalLightIntensity;
    return returns;
}

vec4[2] calculateAllSpotLights(vec3 fragmentPos, vec3 fragmentNormal)
{
    vec4 total[2];
    for(int i = 0; i < spotLight_count; i++)
    {
        vec4 vals[2] = findSpotLight(i, fragmentPos, fragmentNormal);
        total[0] += vals[0];
        total[1] += vals[1];
    }
    return total;
}

void main()
{
    vec4 tex =  texture(textureSampler1, vUV);
    float depth = texture(textureSampler4, vUV).r;
    float f = 100.0;
    float n = 0.01;
    depth = (2 * n) / (f + n - depth * (f - n));

    if(depth >= 0.999)
    {
        outColour = vec4(tex.rgb,tex.a);
    }
    else
    {
        vec3 norm = texture(textureSampler2, vUV).rgb;
        vec3 pos =  texture(textureSampler3, vUV).rgb;

        vec4[2] directionalLighting = calculateAllDirectionalLights(pos, norm);
        vec4[2] pointLighting = calculateAllPointLights(pos, norm);
        vec4[2] spotLighting = calculateAllSpotLights(pos, norm);

        vec3 diffuseLighting = vec3(0);
        diffuseLighting += directionalLighting[0].xyz;
        diffuseLighting += pointLighting[0].xyz;
        diffuseLighting += spotLighting[0].xyz;

        vec4 specularLighting = vec4(0);
        specularLighting += directionalLighting[1];
        specularLighting += pointLighting[1];
        specularLighting += spotLighting[1];

        outColour = vec4(tex.rgb*diffuseLighting,tex.a) + vec4(specularLighting);
    }}
