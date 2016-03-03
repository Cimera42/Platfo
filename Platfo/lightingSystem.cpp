#include "lightingSystem.h"

#include <iostream>
#include "worldComponent.h"
#include "directionalLightComponent.h"
#include "pointLightComponent.h"
#include "globals.h"

SystemID LightingSystem::ID;

LightingSystem::LightingSystem()
{
    std::vector<ComponentID> subList1;
    //Components needed to subscribe to system
    subList1.push_back(WorldComponent::getStaticID());
    subList1.push_back(DirectionalLightComponent::getStaticID());
    addSubList(subList1);

    std::vector<ComponentID> subList2;
    //Components needed to subscribe to system
    subList2.push_back(WorldComponent::getStaticID());
    subList2.push_back(PointLightComponent::getStaticID());
    addSubList(subList2);
}
LightingSystem::~LightingSystem(){}

DirectionalLightGroup LightingSystem::compileDirectional()
{
    DirectionalLightGroup directionalLights;

    for(int subID = 0; subID < subscribedEntities[0].size(); subID++)
    {
        Entity* entity = entities[subscribedEntities[0][subID]];
        WorldComponent* worldComp = static_cast<WorldComponent*>(entity->getComponent(WorldComponent::getStaticID()));
        DirectionalLightComponent* directionalLightComp = static_cast<DirectionalLightComponent*>(entity->getComponent(DirectionalLightComponent::getStaticID()));

        directionalLights.direction.push_back(worldComp->position);
        directionalLights.intensity.push_back(directionalLightComp->intensity);
        directionalLights.colour.push_back(directionalLightComp->colour);
    }
    return directionalLights;
}

PointLightGroup LightingSystem::compilePoint()
{
    PointLightGroup pointLights;

    for(int subID = 0; subID < subscribedEntities[1].size(); subID++)
    {
        Entity* entity = entities[subscribedEntities[1][subID]];
        WorldComponent* worldComp = static_cast<WorldComponent*>(entity->getComponent(WorldComponent::getStaticID()));
        PointLightComponent* pointLightComp = static_cast<PointLightComponent*>(entity->getComponent(PointLightComponent::getStaticID()));

        pointLights.location.push_back(worldComp->position);
        pointLights.intensity.push_back(pointLightComp->intensity);
        pointLights.attenuation.push_back(pointLightComp->attenuation);
        pointLights.colour.push_back(pointLightComp->colour);
    }
    return pointLights;
}
