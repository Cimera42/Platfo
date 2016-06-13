#include "spotLightComponent.h"

ComponentID SpotLightComponent::ID;

SpotLightComponent::SpotLightComponent() {vanityName = "Spot Light Component";}
SpotLightComponent::~SpotLightComponent(){}
SpotLightComponent* SpotLightComponent::construct(Json::Value inValue)
{
    intensity = inValue["intensity"].asDouble();
    attenuation = inValue["attenuation"].asDouble();
    angle = glm::vec2(inValue["angle"][0].asDouble(), inValue["angle"][1].asDouble());
    colour = glm::vec3(inValue["colour"][0].asDouble(),
                       inValue["colour"][1].asDouble(),
                       inValue["colour"][2].asDouble());

    return this;
}
