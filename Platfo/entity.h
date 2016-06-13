#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <unordered_map>
#include <string>
#include "globals.h"

typedef int EntityID;
typedef int ComponentID;

class Component;
class Entity
{
    public:
        Entity();
        ~Entity();

        std::string vanityName = "Unnamed Entity";

        //Incrementor for ids
        static EntityID globalEntityIncrementorID;
        //Unique id for entity
        EntityID entityID;
        EntityID getID();

        EntityID parentEntity;
        std::unordered_map<EntityID, Entity*> childEntities;

        //List of components attached to entity
        std::unordered_map<ComponentID, Component*> components;

        bool addComponent(Component*);
        bool hasComponent(ComponentID);
        bool canUseComponent(ComponentID);
        Component* getComponent(ComponentID);
        bool deleteComponent(ComponentID);

        bool addChild(Entity*);
        bool removeChild(EntityID);
        bool isChild(EntityID);
};

#endif // ENTITY_H_INCLUDED
