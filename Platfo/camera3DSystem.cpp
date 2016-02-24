#include "camera3DSystem.h"

#include "globals.h"
#include "worldComponent.h"
#include "camera3DComponent.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "openGLFunctions.h"
#include "mouseHandler.h"
#include "windowComponent.h"

SystemID Camera3DSystem::ID;

Camera3DSystem::Camera3DSystem()
{
    std::vector<ComponentID> subList1;
    //Components needed to subscribe to system
    subList1.push_back(WorldComponent::getStaticID());
    subList1.push_back(Camera3DComponent::getStaticID());
    addSubList(subList1);

    activeCamera = -1;
}
Camera3DSystem::~Camera3DSystem(){}

void Camera3DSystem::entitySubscribed(Entity* inEntity, int listID)
{
    Camera3DComponent* cameraComp = static_cast<Camera3DComponent*>(inEntity->getComponent(Camera3DComponent::getStaticID()));

    if(cameraComp->activeFlag)
    {
        setActiveCamera(inEntity->entityID);
    }
}

void Camera3DSystem::update()
{
    if(activeCamera == -1)
    {
        if(subscribedEntities[0].size())
        {
            setActiveCamera(subscribedEntities[0][0]);
        }
    }
    if(activeCamera != -1)
    {
        Entity* activeCameraEntity = entities[activeCamera];
        Camera3DComponent* cameraComp = static_cast<Camera3DComponent*>(activeCameraEntity->getComponent(Camera3DComponent::getStaticID()));
        WorldComponent* camWorldComp = static_cast<WorldComponent*>(activeCameraEntity->getComponent(WorldComponent::getStaticID()));

        camWorldComp->rotation.x += (mouseData.yScreenPos-mainWindow->windowSize.y/2)/10;
        camWorldComp->rotation.y -= (mouseData.xScreenPos-mainWindow->windowSize.x/2)/10;
        camWorldComp->updateMatrix();
        cameraComp->updateMatrix();
    }

    /*for(int subID = 0; subID < subscribedEntities[0].size(); subID++)
    {
        Entity * entity = entities[subscribedEntities[0][subID]];

        WorldComponent* worldComp = static_cast<WorldComponent*>(entity->getComponent(WorldComponent::getStaticID()));
        Camera3DComponent* cameraComp = static_cast<Camera3DComponent*>(entity->getComponent(Camera3DComponent::getStaticID()));

        cameraComp->jointMatrix = cameraComp->projectionMatrix;
    }*/
}

void Camera3DSystem::setActiveCamera(EntityID inEntityID)
{
    for(int subID = 0; subID < subscribedEntities[0].size(); subID++)
    {
        Entity * entity = entities[subscribedEntities[0][subID]];

        Camera3DComponent* cameraComp = static_cast<Camera3DComponent*>(entity->getComponent(Camera3DComponent::getStaticID()));

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
