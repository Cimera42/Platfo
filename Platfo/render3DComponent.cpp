#include "render3DComponent.h"
#include "logger.h"
#include "typeConversion.h"
#include "loader.h"
#include "loadShader.h"
#include "mesh.h"

ComponentID Render3DComponent::ID;

Render3DComponent::Render3DComponent(){vanityName = "Render 3D Component";}
Render3DComponent::~Render3DComponent()
{
    glDeleteShader(shader);

    Unload<ModelStore>::Object(&modelStore);
    Unload<TextureStore>::Object(&textureStore);
}

Render3DComponent* Render3DComponent::construct(std::string modelStorePath, std::string textureStorePath)
{
    Load<ModelStore>::Object(&modelStore, modelStorePath);
    Load<TextureStore>::Object(&textureStore, textureStorePath);

    std::vector<const char*> shaderLocations;
    //shaderLocations.push_back("vertPos");
    //shaderLocations.push_back("vertUV");
    //shaderLocations.push_back("vertNorm");
    shader = loadShader("shaders/3dvert.vert", "shaders/3dfrag.frag", shaderLocations);

    //Load texture
    textureLoc = glGetUniformLocation(shader, "textureSampler");
    viewMatLoc = glGetUniformLocation(shader, "viewMat");
    projMatLoc = glGetUniformLocation(shader, "projMat");
    modelMatLoc = glGetUniformLocation(shader, "modelMat");

    return this;
}
Render3DComponent* Render3DComponent::construct(std::vector<std::string> inArgs)
{
    if(inArgs.size() == 0)
    {
        std::string modelPath = inArgs[0];
        std::string texturePath = inArgs[0];

        if(modelPath.length() > 0)
            this->construct(modelPath,texturePath);
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
