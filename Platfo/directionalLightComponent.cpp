#include "directionalLightComponent.h"

ComponentID DirectionalLightComponent::ID;

DirectionalLightComponent::DirectionalLightComponent() {vanityName = "Directional Light Component";}
DirectionalLightComponent::~DirectionalLightComponent(){}
DirectionalLightComponent* DirectionalLightComponent::construct(Json::Value inValue)
{
    intensity = inValue["intensity"].asDouble();
    colour = glm::vec3(inValue["colour"][0].asDouble(),
                       inValue["colour"][1].asDouble(),
                       inValue["colour"][2].asDouble());

    return this;
}
