#include "render3DComponent.h"
#include "logger.h"
#include "typeConversion.h"
#include "loader.h"
#include "mesh.h"

ComponentID Render3DComponent::ID;

Render3DComponent::Render3DComponent(){vanityName = "Render 3D Component";}
Render3DComponent::~Render3DComponent()
{
    Unload<ModelStore>::Object(&modelStore);
    Unload<TextureStore>::Object(&textureStore);
    Unload<ShaderStore>::Object(&shaderStore);
}

Render3DComponent* Render3DComponent::construct(std::string modelStorePath, std::string textureStorePath, std::string shaderStorePath)
{
    Load<ModelStore>::Object(&modelStore, true, modelStorePath);
    Load<TextureStore>::Object(&textureStore, true, textureStorePath);
    Load<ShaderStore>::Object(&shaderStore, true, shaderStorePath);

    textureLoc = -1;
    modelMatLoc = -1;
    viewMatLoc = -1;
    projMatLoc = -1;

    return this;
}
Render3DComponent* Render3DComponent::construct(std::vector<std::string> inArgs)
{
    if(inArgs.size() == 0)
    {
        std::string modelPath = inArgs[0];
        std::string texturePath = inArgs[1];
        std::string shaderPath = inArgs[2];

        if(modelPath != "" && texturePath != "" && shaderPath != "")
            this->construct(modelPath,texturePath,shaderPath);
    }
    else
    {
        Logger() << "Invalid number of arguments to Render 3D Component creation" << std::endl;
    }

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
