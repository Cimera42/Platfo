#include "render2DComponent.h"
#include "loadingSystem.h"

ComponentID Render2DComponent::ID;

Render2DComponent::Render2DComponent(){vanityName = "Render 2D Component";}
Render2DComponent::~Render2DComponent()
{
    //Unload<TextureStore>::Object(&textureStore);
    //Unload<ShaderStore>::Object(&shaderStore);
}
Render2DComponent* Render2DComponent::construct(Json::Value inValue)
{
    LoadingSystem* loadingSys = static_cast<LoadingSystem*>(systems[LoadingSystem::getStaticID()]);
    loadingSys->load<TextureStore>(&textureStore, inValue["textureStore"][0]);
    loadingSys->load<ShaderStore>(&shaderStore, inValue["shaderStore"][0]);

    modelMatLoc = -1;
    viewMatLoc = -1;
    projMatLoc = -1;

    return this;
}

void Render2DComponent::findShaderLocations()
{
    modelMatLoc = glGetUniformLocation(shaderStore->shaderID, "modelMat");
    viewMatLoc = glGetUniformLocation(shaderStore->shaderID, "viewMat");
    projMatLoc = glGetUniformLocation(shaderStore->shaderID, "projMat");
}
