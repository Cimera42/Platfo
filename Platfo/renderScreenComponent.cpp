#include "renderScreenComponent.h"
#include "loadingSystem.h"
#include "render3DSystem.h"

ComponentID RenderScreenComponent::ID;

RenderScreenComponent::RenderScreenComponent(){vanityName = "Render Screen Component";}
RenderScreenComponent::~RenderScreenComponent()
{
    //Unload<TextureStore>::Object(&textureStore);
    //Unload<ShaderStore>::Object(&shaderStore);
}
RenderScreenComponent* RenderScreenComponent::construct(Json::Value inValue)
{
    LoadingSystem* loadingSys = static_cast<LoadingSystem*>(systems[LoadingSystem::getStaticID()]);
    //loadingSys->load<TextureStore>(&textureStore, inValue["textureStore"][0]);
    loadingSys->load<ShaderStore>(&shaderStore, inValue["shaderStore"][0]);

    Render3DSystem* renderSys = static_cast<Render3DSystem*>(systems[Render3DSystem::getStaticID()]);
    frameBufferTextures = renderSys->frameBufferTextures;

    cameraPositionLoc = -1;

    directionalLightLoc_count = -1;
    directionalLightLoc_direction = -1;
    directionalLightLoc_intensity = -1;
    directionalLightLoc_colour = -1;

    pointLightLoc_count = -1;
    pointLightLoc_location = -1;
    pointLightLoc_intensity = -1;
    pointLightLoc_attenuation = -1;
    pointLightLoc_colour = -1;

    spotLightLoc_count = -1;
    spotLightLoc_location = -1;
    spotLightLoc_direction = -1;
    spotLightLoc_intensity = -1;
    spotLightLoc_attenuation = -1;
    spotLightLoc_angle = -1;
    spotLightLoc_colour = -1;

    return this;
}

void RenderScreenComponent::findShaderLocations()
{
    cameraPositionLoc = glGetUniformLocation(shaderStore->shaderID, "cameraPosition");

    directionalLightLoc_count = glGetUniformLocation(shaderStore->shaderID, "directionalLight_count");
    directionalLightLoc_direction = glGetUniformLocation(shaderStore->shaderID, "directionalLight_direction");
    directionalLightLoc_intensity = glGetUniformLocation(shaderStore->shaderID, "directionalLight_intensity");
    directionalLightLoc_colour = glGetUniformLocation(shaderStore->shaderID, "directionalLight_colour");

    pointLightLoc_count = glGetUniformLocation(shaderStore->shaderID, "pointLight_count");
    pointLightLoc_location = glGetUniformLocation(shaderStore->shaderID, "pointLight_location");
    pointLightLoc_intensity = glGetUniformLocation(shaderStore->shaderID, "pointLight_intensity");
    pointLightLoc_attenuation = glGetUniformLocation(shaderStore->shaderID, "pointLight_attenuation");
    pointLightLoc_colour = glGetUniformLocation(shaderStore->shaderID, "pointLight_colour");

    spotLightLoc_count = glGetUniformLocation(shaderStore->shaderID, "spotLight_count");
    spotLightLoc_location = glGetUniformLocation(shaderStore->shaderID, "spotLight_location");
    spotLightLoc_direction = glGetUniformLocation(shaderStore->shaderID, "spotLight_direction");
    spotLightLoc_intensity = glGetUniformLocation(shaderStore->shaderID, "spotLight_intensity");
    spotLightLoc_attenuation = glGetUniformLocation(shaderStore->shaderID, "spotLight_attenuation");
    spotLightLoc_angle = glGetUniformLocation(shaderStore->shaderID, "spotLight_angle");
    spotLightLoc_colour = glGetUniformLocation(shaderStore->shaderID, "spotLight_colour");
}
