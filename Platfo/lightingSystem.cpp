#include "lightingSystem.h"

#include <iostream>
#include "worldComponent.h"
#include "directionalLightComponent.h"
#include "pointLightComponent.h"
#include "spotLightComponent.h"
#include "globals.h"
#include "own_funcs.h"

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

    std::vector<ComponentID> subList3;
    //Components needed to subscribe to system
    subList3.push_back(WorldComponent::getStaticID());
    subList3.push_back(SpotLightComponent::getStaticID());
    addSubList(subList3);
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

        directionalLights.count++;
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

        pointLights.count++;
    }
    return pointLights;
}

SpotLightGroup LightingSystem::compileSpot()
{
    SpotLightGroup spotLights;

    for(int subID = 0; subID < subscribedEntities[2].size(); subID++)
    {
        Entity* entity = entities[subscribedEntities[2][subID]];
        WorldComponent* worldComp = static_cast<WorldComponent*>(entity->getComponent(WorldComponent::getStaticID()));
        SpotLightComponent* spotLightComp = static_cast<SpotLightComponent*>(entity->getComponent(SpotLightComponent::getStaticID()));

        spotLights.location.push_back(worldComp->position);

        float pitch = toRad(worldComp->rotation.x);
        float yaw = toRad(worldComp->rotation.y);
        glm::vec3 direction = glm::vec3(cos(pitch) * sin(yaw),
                                       sin(pitch),
                                       cos(pitch) * cos(yaw));

        spotLights.direction.push_back(direction);
        spotLights.intensity.push_back(spotLightComp->intensity);
        spotLights.attenuation.push_back(spotLightComp->attenuation);
        glm::vec2 angles = glm::vec2(cos(toRad(spotLightComp->angle.x)), cos(toRad(spotLightComp->angle.y)));
        spotLights.angle.push_back(angles);
        spotLights.colour.push_back(spotLightComp->colour);

        spotLights.count++;
    }
    return spotLights;
}
