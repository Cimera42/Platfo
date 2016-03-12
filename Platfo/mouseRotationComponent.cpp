#include "mouseRotationComponent.h"
#include "logger.h"
#include "typeConversion.h"

ComponentID MouseRotationComponent::ID;

MouseRotationComponent::MouseRotationComponent(){vanityName = "Mouse Rotation Component";}
MouseRotationComponent::~MouseRotationComponent(){}
MouseRotationComponent* MouseRotationComponent::construct(float inPitchMin, float inPitchMax, float inYawMin, float inYawMax)
{
    pitchMin = inPitchMin;
    pitchMax = inPitchMax;

    yawMin = inYawMin;
    yawMax = inYawMax;

    return this;
}
MouseRotationComponent* MouseRotationComponent::construct(std::vector<std::string> inArgs)
{
    if(inArgs.size() == 4)
    {
        float pitMin = stringToFloat(inArgs[0]);
        float pitMax = stringToFloat(inArgs[1]);

        float yaMin = stringToFloat(inArgs[2]);
        float yaMax = stringToFloat(inArgs[3]);

        if(pitMin != -9999 && pitMax != -9999 &&
           yaMin != -9999 && yaMax != -9999)
        {
            this->construct(pitMin, pitMax, yaMin, yaMax);
        }
        else
        {
            Logger() << "Invalid input to Mouse Rotation Component creation" << std::endl;
        }
    }
    else
    {
        Logger() << "Invalid number of arguments to Mouse Rotation Component creation" << std::endl;
    }

    return this;
}
