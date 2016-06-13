#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#define MULTITHREADED_LOADING true

#include "logger.h"
#include "typeConversion.h"
#include "own_funcs.h"
#include <unordered_map>

typedef int EntityID;
typedef int ComponentID;
typedef int SystemID;

class Component;
class Entity;
class System;

extern void * __gxx_personality_v0;
extern void * _Unwind_Resume;

extern bool shouldExit;
extern float lastFrame;

extern bool outputFPS;
extern bool outputSub;
extern bool outputDel;

extern std::unordered_map<SystemID, System*> systems;
extern std::unordered_map<ComponentID, Component*> components;
extern std::unordered_map<EntityID, Entity*> entities;

void addEntity(Entity*);
bool entityExists(EntityID);
void deleteEntity(EntityID);
void deleteFlaggedEntities();
void deleteAllEntities();
void deleteAllSystems();


#endif // GLOBALS_H_INCLUDED
