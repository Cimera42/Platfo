#include "tempplayerControlSystem.h"
#include "globals.h"
#include "openGLFunctions.h"

#include "keyboardHandler.h"
#include "mouseHandler.h"
#include "render2DComponent.h"

#include "worldComponent.h"
#include "physicsComponent.h"
#include "tempplayerControlComponent.h"
#include "own_funcs.h"

SystemID PlayerControlSystem::ID;

PlayerControlSystem::PlayerControlSystem()
{
    std::vector<ComponentID> subList1;
    //Components needed to subscribe to system
    subList1.push_back(WorldComponent::getStaticID());
    subList1.push_back(PlayerControlComponent::getStaticID());
    addSubList(subList1);
}
PlayerControlSystem::~PlayerControlSystem(){}

void PlayerControlSystem::update(float inDelta)
{
    for(int subID = 0; subID < subscribedEntities[0].size(); subID++)
    {
        Entity * entity = entities[subscribedEntities[0][subID]];

        WorldComponent* worldComp = static_cast<WorldComponent*>(entity->getComponent(WorldComponent::getStaticID()));
        PlayerControlComponent* controlComp = static_cast<PlayerControlComponent*>(entity->getComponent(PlayerControlComponent::getStaticID()));

        glm::vec3 moveDir = glm::vec3(0,0,0);
        //Check each key
        if(isKeyPressed(controlComp->forwardKey))
        {
            moveDir.z += 1;
        }
        if(isKeyPressed(controlComp->backKey))
        {
            moveDir.z -= 1;
        }
        if(isKeyPressed(controlComp->leftKey))
        {
            moveDir.x -= 1;
        }
        if(isKeyPressed(controlComp->rightKey))
        {
            moveDir.x += 1;
        }
        if(isKeyPressed(controlComp->upKey))
        {
            moveDir.y += 1;
        }
        if(isKeyPressed(controlComp->downKey))
        {
            moveDir.y -= 1;
        }
        //moveDir = glm::normalize(moveDir);

        //moveDir = glm::vec3(rotateVec2(glm::vec2(moveDir.x,moveDir.y), toRad(worldComp->rotation)),0);

        if(entity->canUseComponent(PhysicsComponent::getStaticID()))
        {
            PhysicsComponent* motionComp = static_cast<PhysicsComponent*>(entity->getComponent(PhysicsComponent::getStaticID()));

            motionComp->impulse(moveDir * controlComp->speed * inDelta);
        }
        else
        {
            worldComp->offsetPosition += worldComp->forward * moveDir.z * controlComp->speed * inDelta;//bad!
            worldComp->offsetPosition += worldComp->up * moveDir.y * controlComp->speed * inDelta;//bad!
            worldComp->offsetPosition += worldComp->right * moveDir.x * controlComp->speed * inDelta;//bad!
            worldComp->updateData();
        }
    }
}
