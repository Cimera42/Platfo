#include "camera2DSystem.h"
#include "worldComponent.h"
#include "camera2DComponent.h"

SystemID Camera2DSystem::ID;

Camera2DSystem::Camera2DSystem()
{
    std::vector<ComponentID> subList1;
    //Components needed to subscribe to system
    subList1.push_back(WorldComponent::getStaticID());
    subList1.push_back(Camera2DComponent::getStaticID());
    addSubList(subList1);

    activeCamera = -1;
}
Camera2DSystem::~Camera2DSystem(){}

void Camera2DSystem::entitySubscribed(Entity* inEntity, int listID)
{
    Camera2DComponent* cameraComp = static_cast<Camera2DComponent*>(inEntity->getComponent(Camera2DComponent::getStaticID()));

    if(cameraComp->activeFlag)
    {
        setActiveCamera(inEntity->entityID);
    }
}

void Camera2DSystem::update()
{
    if(activeCamera == -1)
    {
        if(subscribedEntities[0].size())
        {
            setActiveCamera(subscribedEntities[0][0]);
        }
    }
}

void Camera2DSystem::setActiveCamera(EntityID inEntityID)
{
    for(int subID = 0; subID < subscribedEntities[0].size(); subID++)
    {
        Entity * entity = entities[subscribedEntities[0][subID]];

        Camera2DComponent* cameraComp = static_cast<Camera2DComponent*>(entity->getComponent(Camera2DComponent::getStaticID()));

        if(inEntityID == entity->entityID)
        {
            cameraComp->activeFlag = true;
            activeCamera = entity->entityID;
        }
        else
        {
            cameraComp->activeFlag = false;
        }
    }
}
