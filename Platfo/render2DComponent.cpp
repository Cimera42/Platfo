#include "render2DComponent.h"
#include "logger.h"
#include "typeConversion.h"
#include "loader.h"

ComponentID Render2DComponent::ID;

Render2DComponent::Render2DComponent(){vanityName = "Render 2D Component";}
Render2DComponent::~Render2DComponent()
{
    Unload<TextureStore>::Object(&textureStore);
    Unload<ShaderStore>::Object(&shaderStore);
}
Render2DComponent* Render2DComponent::construct(std::string textureStoreName, std::string shaderStoreName)
{
    Load<TextureStore>::Object(&textureStore, textureStoreName);
    Load<ShaderStore>::Object(&shaderStore, shaderStoreName);

    textureLoc = -1;
    modelMatLoc = -1;
    viewMatLoc = -1;
    projMatLoc = -1;

    return this;
}
Render2DComponent* Render2DComponent::construct(std::vector<std::string> inArgs)
{
    if(inArgs.size() == 2)
    {
        std::string textureStoreName = inArgs[0];
        std::string shaderStoreName = inArgs[1];

        if(textureStoreName != "" && shaderStoreName != "")
        {
            this->construct(textureStoreName, shaderStoreName);
        }
        else
        {
            Logger() << "Invalid input to Render 2D Component creation" << std::endl;
        }
    }
    else
    {
        Logger() << "Invalid number of arguments to Render 2D Component creation" << std::endl;
    }

    return this;
}

void Render2DComponent::findShaderLocations()
{
    textureLoc = glGetUniformLocation(shaderStore->shaderID, "textureSampler");
    modelMatLoc = glGetUniformLocation(shaderStore->shaderID, "modelMat");
    viewMatLoc = glGetUniformLocation(shaderStore->shaderID, "viewMat");
    projMatLoc = glGetUniformLocation(shaderStore->shaderID, "projMat");
}
