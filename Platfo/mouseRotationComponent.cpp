#include "mouseRotationComponent.h"

ComponentID MouseRotationComponent::ID;

MouseRotationComponent::MouseRotationComponent(){vanityName = "Mouse Rotation Component";}
MouseRotationComponent::~MouseRotationComponent(){}
MouseRotationComponent* MouseRotationComponent::construct(Json::Value inValue)
{
    pitchMin = inValue["pitchMin"].asDouble();
    pitchMax = inValue["pitchMax"].asDouble();

    yawMin = inValue["yawMin"].asDouble();
    yawMax = inValue["yawMax"].asDouble();

    return this;
}
