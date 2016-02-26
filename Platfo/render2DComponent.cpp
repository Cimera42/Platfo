#include "render2DComponent.h"
#include "logger.h"
#include "typeConversion.h"
#include "loader.h"
#include "loadShader.h"

ComponentID Render2DComponent::ID;

Render2DComponent::Render2DComponent(){vanityName = "Render 2D Component";}
Render2DComponent::~Render2DComponent()
{
    glDeleteShader(shader);

    Unload<TextureStore>::Object(&textureStore);
}
Render2DComponent* Render2DComponent::construct(std::string textureStoreName)
{
    Load<TextureStore>::Object(&textureStore, textureStoreName);

    //Create shader
    std::vector<const char*> shaderLocations;
    //shaderLocations.push_back("vertPos");
    //shaderLocations.push_back("vertUV");
    shader = loadShader("shaders/2dvert.vert", "shaders/2dfrag.frag", shaderLocations);

    textureLoc = glGetUniformLocation(shader, "textureSampler");
    modelMatLoc = glGetUniformLocation(shader, "modelMat");
    viewMatLoc = glGetUniformLocation(shader, "viewMat");
    projMatLoc = glGetUniformLocation(shader, "projMat");

    return this;
}
Render2DComponent* Render2DComponent::construct(std::vector<std::string> inArgs)
{
    if(inArgs.size() == 4)
    {
        std::string textureStoreName = inArgs[0];

        if(textureStoreName != "")
        {
            this->construct(textureStoreName);
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
