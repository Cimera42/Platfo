#include "pointLightComponent.h"
#include "logger.h"
#include "typeConversion.h"

ComponentID PointLightComponent::ID;

PointLightComponent::PointLightComponent() {vanityName = "Point Light Component";}
PointLightComponent::~PointLightComponent(){}
PointLightComponent* PointLightComponent::construct(float inIntensity, float inAttenuation, glm::vec3 inColour)
{
    intensity = inIntensity;
    attenuation = inAttenuation;
    colour = inColour;

    return this;
}
PointLightComponent* PointLightComponent::construct(std::vector<std::string> inArgs)
{
    if(inArgs.size() == 5)
    {
        float inten = stringToFloat(inArgs[0]);
        float atten = stringToFloat(inArgs[1]);
        float r = stringToFloat(inArgs[2]);
        float g = stringToFloat(inArgs[3]);
        float b = stringToFloat(inArgs[4]);

        if(inten != -9999 && atten != -9999 &&
           r != -9999 && g != -9999 && b != -9999)
        {
            glm::vec3 col = glm::vec3(r,g,b);
            this->construct(inten,atten,col);
        }
        else
        {
            Logger() << "Invalid input to Point Light Component creation" << std::endl;
        }
    }
    else
    {
        Logger() << "Invalid number of arguments to Point Light Component creation" << std::endl;
    }

    return this;
}
