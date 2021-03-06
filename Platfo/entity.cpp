#include "entity.h"
#include "component.h"
#include "system.h"

#include <stdlib.h>

EntityID Entity::globalEntityIncrementorID = 0;
Entity::Entity()
{
    //Give entity unique id
    entityID = globalEntityIncrementorID++;

    parentEntity = -1;
}
Entity::~Entity()
{
    std::vector<ComponentID> componentToErase;
    for(std::unordered_map<ComponentID, Component*>::iterator componentPair = components.begin(); componentPair != components.end(); ++componentPair)
    {
        //please add to list then delete.
        delete componentPair->second;
        componentToErase.push_back(componentPair->first);
    }
    for (int i = 0; i < (int) componentToErase.size(); i++)
        components.erase(componentToErase[i]);
}

EntityID Entity::getID()
{
    return entityID;
}

//Add component to entity
bool Entity::addComponent(Component* inComponent)
{
    //Get id of component
    ComponentID compID = inComponent->getID();
    //Check if entity already has component
    if(!hasComponent(compID))
    {
        //Add component
        components[compID] = inComponent;
        inComponent->parentEntity = entityID;
        //Subscribe to systems
        subscribeToSystems(this, compID);
        return true;
    }
    return false;
}

//Check if component exists in the list
bool Entity::hasComponent(ComponentID compID)
{
    return components.find(compID) != components.end();
}

//Check if component is available for use
bool Entity::canUseComponent(ComponentID compID)
{
    //Check if component exists
    if(hasComponent(compID))
    {
        //Check if component is enabled
        if(getComponent(compID)->enabled)
        {
            return true;
        }
    }
    return false;
}

//Get component from entity
Component* Entity::getComponent(ComponentID compID)
{
    //Check if component exists
    if(hasComponent(compID))
    {
        return components[compID];
    }
    return NULL;
}

//Delete component from entity
bool Entity::deleteComponent(ComponentID compID)
{
    //Check if component exists
    if(hasComponent(compID))
    {
        //Unsubscribe to systems
        unsubscribeToSystemsByComponent(this, compID);
        //Delete object
        delete components[compID];
        //Remove empty pointer from list of components
        components.erase(compID);
    }
    return false;
}

bool Entity::addChild(Entity* childEntity)
{
    if(entityExists(childEntity->entityID))
    {
        if(!isChild(childEntity->entityID))
        {
            childEntity->parentEntity = entityID;
            childEntities[childEntity->entityID] = childEntity;
        }
    }
}

bool Entity::removeChild(EntityID childEntityID)
{
    if(isChild(childEntityID))
    {
        childEntities.erase(childEntityID);
    }
}

bool Entity::isChild(EntityID childEntityID)
{
    return childEntities.find(childEntityID) != childEntities.end();
}
