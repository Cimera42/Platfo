#include "globals.h"
#include "entity.h"
#include "component.h"
#include "system.h"
#include <pthread.h>

void * __gxx_personality_v0=0;
void * _Unwind_Resume =0;

bool shouldExit = false;

bool outputFPS = false;
bool outputSub = false;
bool outputDel = false;

float lastFrame = 0;

std::unordered_map<SystemID, System*> systems;
std::unordered_map<ComponentID, Component*> components;
pthread_mutex_t entityMutex = PTHREAD_MUTEX_INITIALIZER;
std::unordered_map<EntityID, Entity*> entities;
std::vector<EntityID> entitiesToDelete;

void addEntity(Entity* inEntity)
{
    EntityID id = inEntity->getID();
    pthread_mutex_lock(&entityMutex);
    entities[id] = inEntity;
    pthread_mutex_unlock(&entityMutex);
}

bool entityExists(EntityID entityID)
{
    return entities.find(entityID) != entities.end();
}

void deleteEntity(EntityID inID)
{
    if(entityExists(inID))
        entitiesToDelete.push_back(inID);
}

void deleteFlaggedEntities()
{
    int entCount = 0;
    for(int i = 0; i < entitiesToDelete.size(); i++)
    {
        EntityID id = entitiesToDelete[i];
        unsubscribeToSystems(entities[id]);
        delete entities[id];
        if(outputDel)
            Logger() << "Entity " << id << " deleted" << std::endl;
        entities.erase(id);
        entCount++;
    }
    if(entCount)
    {
        if(outputDel)
            Logger() << entCount << " entities deleted" << std::endl;
    }
    std::vector<EntityID>().swap(entitiesToDelete);
}

void deleteAllEntities()
{
    std::vector<EntityID>().swap(entitiesToDelete);
    for(std::unordered_map<EntityID, Entity*>::iterator entityPair = entities.begin(); entityPair != entities.end(); ++entityPair)
    {
        entitiesToDelete.push_back(entityPair->first);
    }
    deleteFlaggedEntities();
}

void deleteAllSystems()
{
    std::vector<SystemID> toDelete;
    for(std::unordered_map<SystemID, System*>::iterator systemPair = systems.begin(); systemPair != systems.end(); ++systemPair)
    {
        toDelete.push_back(systemPair->first);
    }

    int sysCount = 0;
    for(int i = 0; i < toDelete.size(); i++)
    {
        delete systems[toDelete[i]];
        Logger() << "System " << toDelete[i] << " deleted" << std::endl;
        systems.erase(toDelete[i]);
        sysCount++;
    }
    Logger() << sysCount << " systems deleted" << std::endl;
}
