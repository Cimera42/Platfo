#include "spotLightComponent.h"
#include "logger.h"
#include "typeConversion.h"

ComponentID SpotLightComponent::ID;

SpotLightComponent::SpotLightComponent() {vanityName = "Spot Light Component";}
SpotLightComponent::~SpotLightComponent(){}
SpotLightComponent* SpotLightComponent::construct(float inIntensity, float inAttenuation, glm::vec2 inAngle, glm::vec3 inColour)
{
    intensity = inIntensity;
    attenuation = inAttenuation;
    angle = inAngle;
    colour = inColour;

    return this;
}
SpotLightComponent* SpotLightComponent::construct(std::vector<std::string> inArgs)
{
    if(inArgs.size() == 7)
    {
        float inten = stringToFloat(inArgs[0]);
        float atten = stringToFloat(inArgs[1]);
        float angIn = stringToFloat(inArgs[2]);
        float angOu = stringToFloat(inArgs[3]);
        float r = stringToFloat(inArgs[4]);
        float g = stringToFloat(inArgs[5]);
        float b = stringToFloat(inArgs[6]);

        if(inten != -9999 && atten != -9999 &&
           angIn != -9999 && angOu != -9999 &&
           r != -9999 && g != -9999 && b != -9999)
        {
            glm::vec2 ang = glm::vec2(angIn,angOu);
            glm::vec3 col = glm::vec3(r,g,b);
            this->construct(inten,atten,ang,col);
        }
        else
        {
            Logger() << "Invalid input to Spot Light Component creation" << std::endl;
        }
    }
    else
    {
        Logger() << "Invalid number of arguments to Spot Light Component creation" << std::endl;
    }

    return this;
}
