#include "renderScreenComponent.h"
#include "logger.h"
#include "typeConversion.h"
#include "loader.h"

ComponentID RenderScreenComponent::ID;

RenderScreenComponent::RenderScreenComponent(){vanityName = "Render Screen Component";}
RenderScreenComponent::~RenderScreenComponent()
{
    Unload<TextureStore>::Object(&textureStore);
    Unload<ShaderStore>::Object(&shaderStore);
}
RenderScreenComponent* RenderScreenComponent::construct(std::string textureStoreName, bool doLoadTexture, std::string shaderStoreName)
{
    Load<TextureStore>::Object(&textureStore, true, textureStoreName);
    Load<ShaderStore>::Object(&shaderStore, true, shaderStoreName);

    directionalLightLoc_direction = -1;
    directionalLightLoc_intensity = -1;
    directionalLightLoc_colour = -1;

    pointLightLoc_location = -1;
    pointLightLoc_intensity = -1;
    pointLightLoc_attenuation = -1;
    pointLightLoc_colour = -1;

    return this;
}
RenderScreenComponent* RenderScreenComponent::construct(std::vector<std::string> inArgs)
{
    if(inArgs.size() == 3)
    {
        std::string textureStoreName = inArgs[0];
        bool doLoadTexture = stringToBool(inArgs[1]);
        std::string shaderStoreName = inArgs[2];

        if(textureStoreName != "" && shaderStoreName != "")
        {
            this->construct(textureStoreName, doLoadTexture, shaderStoreName);
        }
        else
        {
            Logger() << "Invalid input to Render Screen Component creation" << std::endl;
        }
    }
    else
    {
        Logger() << "Invalid number of arguments to Render Screen Component creation" << std::endl;
    }

    return this;
}

void RenderScreenComponent::findShaderLocations()
{
    directionalLightLoc_direction = glGetUniformLocation(shaderStore->shaderID, "directionalLight_direction");
    directionalLightLoc_intensity = glGetUniformLocation(shaderStore->shaderID, "directionalLight_intensity");
    directionalLightLoc_colour = glGetUniformLocation(shaderStore->shaderID, "directionalLight_colour");

    pointLightLoc_location = glGetUniformLocation(shaderStore->shaderID, "pointLight_location");
    pointLightLoc_intensity = glGetUniformLocation(shaderStore->shaderID, "pointLight_intensity");
    pointLightLoc_attenuation = glGetUniformLocation(shaderStore->shaderID, "pointLight_attenuation");
    pointLightLoc_colour = glGetUniformLocation(shaderStore->shaderID, "pointLight_colour");
}
