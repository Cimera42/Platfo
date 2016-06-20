#include "render3DComponent.h"
#include "loadingSystem.h"
#include "mesh.h"

ComponentID Render3DComponent::ID;

Render3DComponent::Render3DComponent(){vanityName = "Render 3D Component";}
Render3DComponent::~Render3DComponent()
{
    LoadingSystem* loadingSys = static_cast<LoadingSystem*>(systems[LoadingSystem::getStaticID()]);
    loadingSys->unload(&textureStore);
    loadingSys->unload(&shaderStore);
    loadingSys->unload(&modelStore);
}

Render3DComponent* Render3DComponent::construct(Json::Value inValue)
{
    //Load<ModelStore>::Object(&modelStore, true, inValue["modelStore"]);
    //Load<TextureStore>::Object(&textureStore, true, inValue["textureStore"]);
    //Load<ShaderStore>::Object(&shaderStore, true, inValue["shaderStore"]);
    LoadingSystem* loadingSys = static_cast<LoadingSystem*>(systems[LoadingSystem::getStaticID()]);
    loadingSys->load<ModelStore>(&modelStore, inValue["modelStore"][0]);
    loadingSys->load<TextureStore>(&textureStore, inValue["textureStore"][0]);
    loadingSys->load<ShaderStore>(&shaderStore, inValue["shaderStore"][0]);

    textureLoc = -1;
    modelMatLoc = -1;
    viewMatLoc = -1;
    projMatLoc = -1;

    return this;
}

void Render3DComponent::createVAO()
{
    modelStore->mesh.createVAO();
}

void Render3DComponent::findShaderLocations()
{
    textureLoc = glGetUniformLocation(shaderStore->shaderID, "textureSampler");
    modelMatLoc = glGetUniformLocation(shaderStore->shaderID, "modelMat");
    viewMatLoc = glGetUniformLocation(shaderStore->shaderID, "viewMat");
    projMatLoc = glGetUniformLocation(shaderStore->shaderID, "projMat");
}
