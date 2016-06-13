#include "pointLightComponent.h"
#include "logger.h"
#include "typeConversion.h"

ComponentID PointLightComponent::ID;

PointLightComponent::PointLightComponent() {vanityName = "Point Light Component";}
PointLightComponent::~PointLightComponent(){}
PointLightComponent* PointLightComponent::construct(Json::Value inValue)
{
    intensity = inValue["intensity"].asDouble();
    attenuation = inValue["attenuation"].asDouble();
    colour = glm::vec3(inValue["colour"][0].asDouble(),
                       inValue["colour"][1].asDouble(),
                       inValue["colour"][2].asDouble());

    return this;
}
