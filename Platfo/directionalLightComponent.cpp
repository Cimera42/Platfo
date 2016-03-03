#include "directionalLightComponent.h"
#include "logger.h"
#include "typeConversion.h"\

ComponentID DirectionalLightComponent::ID;

DirectionalLightComponent::DirectionalLightComponent() {vanityName = "Directional Light Component";}
DirectionalLightComponent::~DirectionalLightComponent(){}
DirectionalLightComponent* DirectionalLightComponent::construct(float inIntensity, glm::vec3 inColour)
{
    intensity = inIntensity;
    colour = inColour;

    return this;
}
DirectionalLightComponent* DirectionalLightComponent::construct(std::vector<std::string> inArgs)
{
    if(inArgs.size() == 4)
    {
        float inten = stringToFloat(inArgs[0]);
        float r = stringToFloat(inArgs[1]);
        float g = stringToFloat(inArgs[2]);
        float b = stringToFloat(inArgs[3]);

        if(inten != -9999 &&
           r != -9999 && g != -9999 && b != -9999)
        {
            glm::vec3 col = glm::vec3(r,g,b);
            this->construct(inten,col);
        }
        else
        {
            Logger() << "Invalid input to Directional Light Component creation" << std::endl;
        }
    }
    else
    {
        Logger() << "Invalid number of arguments to Directional Light Component creation" << std::endl;
    }

    return this;
}
