#include "renderSkyboxComponent.h"
#include "logger.h"
#include "typeConversion.h"
#include "loader.h"
#include "mesh.h"

ComponentID RenderSkyboxComponent::ID;

RenderSkyboxComponent::RenderSkyboxComponent(){vanityName = "Render Skybox Component";}
RenderSkyboxComponent::~RenderSkyboxComponent()
{
    Unload<ModelStore>::Object(&modelStore);
    Unload<TextureStore>::Object(&textureStore);
    Unload<ShaderStore>::Object(&shaderStore);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

RenderSkyboxComponent* RenderSkyboxComponent::construct(std::string modelStorePath, std::string textureStorePath, std::string shaderStorePath)
{
    Load<ModelStore>::Object(&modelStore, true, modelStorePath);
    Load<TextureStore>::Object(&textureStore, true, textureStorePath);
    Load<ShaderStore>::Object(&shaderStore, true, shaderStorePath);

    viewMatLoc = -1;
    projMatLoc = -1;
    textureLoc = -1;

    return this;
}
RenderSkyboxComponent* RenderSkyboxComponent::construct(std::vector<std::string> inArgs)
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
        Logger() << "Invalid number of arguments to Render Skybox Component creation" << std::endl;
    }

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
