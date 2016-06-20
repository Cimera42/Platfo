#include "renderSkyboxComponent.h"
#include "loadingSystem.h"
#include "mesh.h"

ComponentID RenderSkyboxComponent::ID;

RenderSkyboxComponent::RenderSkyboxComponent()
{
    vanityName = "Render Skybox Component";
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}
RenderSkyboxComponent::~RenderSkyboxComponent()
{
    LoadingSystem* loadingSys = static_cast<LoadingSystem*>(systems[LoadingSystem::getStaticID()]);
    loadingSys->unload(&textureStore);
    loadingSys->unload(&shaderStore);
    loadingSys->unload(&modelStore);
}

RenderSkyboxComponent* RenderSkyboxComponent::construct(Json::Value inValue)
{
    LoadingSystem* loadingSys = static_cast<LoadingSystem*>(systems[LoadingSystem::getStaticID()]);
    loadingSys->load<ModelStore>(&modelStore, inValue["modelStore"][0]);
    loadingSys->load<TextureStore>(&textureStore, inValue["textureStore"][0]);
    loadingSys->load<ShaderStore>(&shaderStore, inValue["shaderStore"][0]);

    viewMatLoc = -1;
    projMatLoc = -1;
    textureLoc = -1;

    return this;
}

void RenderSkyboxComponent::createVAO()
{
    modelStore->mesh.createVAO();
}

void RenderSkyboxComponent::findShaderLocations()
{
    textureLoc = glGetUniformLocation(shaderStore->shaderID, "skyboxTexture");
    viewMatLoc = glGetUniformLocation(shaderStore->shaderID, "viewMat");
    projMatLoc = glGetUniformLocation(shaderStore->shaderID, "projMat");
}
