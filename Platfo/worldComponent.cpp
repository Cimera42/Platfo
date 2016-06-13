#include "worldComponent.h"
#include <glm/gtx/transform.hpp>

ComponentID WorldComponent::ID;

WorldComponent::WorldComponent(){vanityName = "World Component";}
WorldComponent::~WorldComponent(){}
WorldComponent* WorldComponent::construct(Json::Value inValue)
{
    offsetPosition = glm::vec3(inValue["position"][0].asDouble(),
                               inValue["position"][1].asDouble(),
                               inValue["position"][2].asDouble());
    position = glm::vec3(inValue["position"][0].asDouble(),
                               inValue["position"][1].asDouble(),
                               inValue["position"][2].asDouble());
    scale = glm::vec3(inValue["scale"][0].asDouble(),
                               inValue["scale"][1].asDouble(),
                               inValue["scale"][2].asDouble());
    rotation = glm::vec3(inValue["rotation"][0].asDouble(),
                               inValue["rotation"][1].asDouble(),
                               inValue["rotation"][2].asDouble());
    updateData();

    return this;
}

void WorldComponent::updateData()
{
    updateMatrix();
    updateChildren();
}

void WorldComponent::updateMatrix()
{
    modelMatrix = glm::mat4();

    position = offsetPosition;

    WorldComponent* pComp = getParent();
    if(pComp)
    {
        glm::vec3 pPos = pComp->getParentPosition();
        pPos += pComp->right * offsetPosition.x;
        pPos += pComp->up * offsetPosition.y;
        pPos += pComp->forward * offsetPosition.z;
        position = pPos;
    }

    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, toRad(rotation.y), glm::vec3(0,1,0));
    modelMatrix = glm::rotate(modelMatrix, toRad(-rotation.x), glm::vec3(1,0,0));
    modelMatrix = glm::rotate(modelMatrix, toRad(rotation.z), glm::vec3(0,0,1));
    modelMatrix = glm::scale(modelMatrix, scale);

    glm::vec3 rot = getParentRotation();
    float pitch = toRad(rot.x);
    float yaw = toRad(rot.y);

    forward = glm::vec3(cos(pitch) * sin(yaw),
                             sin(pitch),
                             cos(pitch) * cos(yaw));
    right = glm::vec3(sin(yaw - 3.14f/2.0f),
                0,
                cos(yaw - 3.14f/2.0f));
    up = glm::cross(right, forward);
}

void WorldComponent::updateChildren()
{
    if(parentEntity != -1)
    {
        Entity* ent = entities[parentEntity];
        for(std::unordered_map<EntityID, Entity*>::iterator childPair = ent->childEntities.begin(); childPair != ent->childEntities.end(); ++childPair)
        {
            Entity* childEnt = childPair->second;

            if(childEnt->hasComponent(WorldComponent::getStaticID()))
            {
                WorldComponent* worldComp = static_cast<WorldComponent*>(childEnt->getComponent(WorldComponent::getStaticID()));

                worldComp->updateData();
            }
        }
    }
}

WorldComponent* WorldComponent::getParent()
{
    if(parentEntity != -1)
    {
        Entity* ent = entities[parentEntity];
        if(ent->parentEntity != -1)
        {
            Entity* pent = entities[ent->parentEntity];
            if(pent->hasComponent(WorldComponent::getStaticID()))
            {
                WorldComponent* worldComp = static_cast<WorldComponent*>(pent->getComponent(WorldComponent::getStaticID()));

                return worldComp;
            }
        }
    }
    return NULL;
}

glm::vec3 WorldComponent::getParentPosition()
{
    if(parentEntity != -1)
    {
        Entity* ent = entities[parentEntity];
        if(ent->parentEntity != -1)
        {
            Entity* pent = entities[ent->parentEntity];
            if(pent->hasComponent(WorldComponent::getStaticID()))
            {
                WorldComponent* worldComp = static_cast<WorldComponent*>(pent->getComponent(WorldComponent::getStaticID()));

                return worldComp->getParentPosition() + position;
            }
        }
    }
    return position;
}

glm::vec3 WorldComponent::getParentRotation()
{
    if(parentEntity != -1)
    {
        Entity* ent = entities[parentEntity];
        if(ent->parentEntity != -1)
        {
            Entity* pent = entities[ent->parentEntity];
            if(pent->hasComponent(WorldComponent::getStaticID()))
            {
                WorldComponent* worldComp = static_cast<WorldComponent*>(pent->getComponent(WorldComponent::getStaticID()));

                return worldComp->getParentRotation() + rotation;
            }
        }
    }
    return rotation;
}

glm::mat4 WorldComponent::getParentMatrix()
{
    if(parentEntity != -1)
    {
        Entity* ent = entities[parentEntity];
        if(ent->parentEntity != -1)
        {
            Entity* pent = entities[ent->parentEntity];
            if(pent->hasComponent(WorldComponent::getStaticID()))
            {
                WorldComponent* worldComp = static_cast<WorldComponent*>(pent->getComponent(WorldComponent::getStaticID()));

                return worldComp->getParentMatrix() * modelMatrix;
            }
        }
    }
    return modelMatrix;
}
