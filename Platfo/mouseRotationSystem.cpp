#include "mouseRotationSystem.h"
#include "mouseHandler.h"
#include "windowComponent.h"
#include "worldComponent.h"
#include "mouseRotationComponent.h"

SystemID MouseRotationSystem::ID;

MouseRotationSystem::MouseRotationSystem()
{
    std::vector<ComponentID> subList1;
    //Components needed to subscribe to system
    subList1.push_back(WorldComponent::getStaticID());
    subList1.push_back(MouseRotationComponent::getStaticID());
    addSubList(subList1);
}
MouseRotationSystem::~MouseRotationSystem(){}

void MouseRotationSystem::update(float inDelta)
{
    for(int subID = 0; subID < subscribedEntities[0].size(); subID++)
    {
        Entity * entity = entities[subscribedEntities[0][subID]];

        WorldComponent* worldComp = static_cast<WorldComponent*>(entity->getComponent(WorldComponent::getStaticID()));
        MouseRotationComponent* mouseRotComp = static_cast<MouseRotationComponent*>(entity->getComponent(MouseRotationComponent::getStaticID()));

        worldComp->rotation.x -= (mouseData.yScreenPos-mainWindow->windowSize.y/2)/10;
        if(mouseRotComp->pitchMin != -9999)
        {
            worldComp->rotation.x = glm::max(mouseRotComp->pitchMin, worldComp->rotation.x);
            worldComp->rotation.x = glm::min(mouseRotComp->pitchMax, worldComp->rotation.x);
        }

        worldComp->rotation.y -= (mouseData.xScreenPos-mainWindow->windowSize.x/2)/10;
        if(mouseRotComp->yawMin != -9999)
        {
            worldComp->rotation.y = glm::max(mouseRotComp->yawMin, worldComp->rotation.y);
            worldComp->rotation.y = glm::min(mouseRotComp->yawMax, worldComp->rotation.y);
        }
        worldComp->updateData();
    }
}
