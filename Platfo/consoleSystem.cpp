#include "consoleSystem.h"
#include <string>
#include <iostream>
#include "keyboardHandler.h"
#include "worldComponent.h"

SystemID ConsoleSystem::ID;

ConsoleSystem::ConsoleSystem()
{
    inConsole = false;
}
ConsoleSystem::~ConsoleSystem(){}

void ConsoleSystem::update()
{
    /*
    if(isKeyPressed(GLFW_KEY_GRAVE_ACCENT))
        inConsole = true;
    while(inConsole)
    {
        std::string inStuff;

        Logger() << "Console:";
        std::getline(std::cin, inStuff);

        std::vector<std::string> words = splitBy(inStuff, ' ');

        if(words.size() > 0)
        {
            if(words.size() >= 1)
            {
                if(words[0] == "exit")
                {
                    inConsole = false;
                }
                else if(words[0] == "list")
                {
                    if(words.size() >= 2)
                    {
                        if(words[1] == "count")
                        {
                            Logger() << "Total Entities:" << entities.size() << std::endl;
                            Logger() << "Total Components:" << components.size() << std::endl;
                        }
                        else if(words[1] == "entities")
                        {
                            Logger() << "Entities:" << std::endl;
                            for(std::unordered_map<EntityID, Entity*>::iterator entityPair = entities.begin(); entityPair != entities.end(); ++entityPair)
                            {
                                Logger() << "    " << entityPair->first << " - " << entityPair->second->vanityName << std::endl;
                            }
                            Logger() << "Total:" << entities.size() << std::endl;
                        }
                        else if(words[1] == "components")
                        {
                            if(words.size() == 2)
                            {
                                Logger() << "Components:" << std::endl;
                                for(std::unordered_map<ComponentID, Component*>::iterator compPair = components.begin(); compPair != components.end(); ++compPair)
                                {
                                    Logger() << "    " << compPair->first << " - " << compPair->second->vanityName << std::endl;
                                }
                                Logger() << "Total:" << components.size() << std::endl;
                            }
                            else if(words.size() >= 3)
                            {
                                int number = stringToInt(words[2]);
                                if(number != -9999)
                                {
                                    std::unordered_map<EntityID, Entity*>::iterator searcher = entities.find(number);
                                    if(searcher != entities.end())
                                    {
                                        Logger() << "Entity " << searcher->first << " - " << searcher->second->vanityName << std::endl;
                                        listComponents(searcher->first);
                                    }
                                    else
                                    {
                                        Logger() << "Entity " << number << " does not exist" << std::endl;
                                    }
                                }
                                else if(words[2] == "all")
                                {
                                    for(std::unordered_map<EntityID, Entity*>::iterator entityPair = entities.begin(); entityPair != entities.end(); ++entityPair)
                                    {
                                        Logger() << "Entity " << entityPair->first << " - " << entityPair->second->vanityName << std::endl;
                                        listComponents(entityPair->first);
                                    }
                                }
                                else
                                {
                                    Logger() << "Invalid entity identifier" << std::endl;
                                    Logger() << "  Please use 'all' or an entity number" << std::endl;
                                }
                            }
                        }
                    }
                }
                else if(words[0] == "delete")
                {
                    if(words.size() >= 2)
                    {
                        if(words[1] == "entity")
                        {
                            if(words.size() >= 3)
                            {
                                int number = stringToInt(words[2]);
                                if(number != -9999)
                                {
                                    std::unordered_map<EntityID, Entity*>::iterator searcher = entities.find(number);
                                    if(searcher != entities.end())
                                    {
                                        Logger() << "Entity " << searcher->first << " - " << searcher->second->vanityName << std::endl;
                                        deleteEntity(searcher->first);
                                    }
                                    else
                                    {
                                        Logger() << "Entity " << number << " does not exist" << std::endl;
                                    }
                                }
                            }
                        }
                        else if(words[1] == "component")
                        {
                            if(words.size() >= 3)
                            {
                                int number = stringToInt(words[2]);
                                if(number != -9999)
                                {
                                    std::unordered_map<EntityID, Entity*>::iterator searcher = entities.find(number);
                                    if(searcher != entities.end())
                                    {
                                        Logger() << "Entity " << searcher->first << " - " << searcher->second->vanityName << std::endl;
                                        if(words.size() >= 4)
                                        {
                                            int number = stringToInt(words[3]);
                                            if(number != -9999)
                                            {
                                                std::unordered_map<ComponentID, Component*>::iterator compsearcher = searcher->second->components.find(number);
                                                if(compsearcher != searcher->second->components.end())
                                                {
                                                    Logger() << "Component " << compsearcher->first << " - " << compsearcher->second->vanityName << std::endl;
                                                    searcher->second->deleteComponent(compsearcher->first);
                                                }
                                                else
                                                {
                                                    Logger() << "Component " << number << " does not exist" << std::endl;
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        Logger() << "Entity " << number << " does not exist" << std::endl;
                                    }
                                }
                            }
                        }
                    }
                }
                else if(words[0] == "add")
                {
                    if(words.size() >= 2)
                    {
                        if(words[1] == "entity")
                        {
                            Entity* newEnt = new Entity();
                            addEntity(newEnt);
                            bool name = false;
                            if(words.size() >= 3)
                            {
                                newEnt->vanityName = words[2];
                                name = true;
                            }
                            if(name)
                                Logger() << "Added entity " << newEnt->entityID << " - " << newEnt->vanityName << std::endl;
                            else
                                Logger() << "Added entity " << newEnt->entityID << std::endl;
                        }
                        else if(words[1] == "component")
                        {
                            if(words.size() >= 3)
                            {
                                int number = stringToInt(words[2]);
                                if(number != -9999)
                                {
                                    std::unordered_map<EntityID, Entity*>::iterator searcher = entities.find(number);
                                    if(searcher != entities.end())
                                    {
                                        Logger() << "Entity " << searcher->first << " - " << searcher->second->vanityName << std::endl;
                                        if(words.size() >= 4)
                                        {
                                            int compnumber = stringToInt(words[3]);
                                            if(compnumber != -9999)
                                            {
                                                std::unordered_map<ComponentID, Component*>::iterator compsearcher = components.find(compnumber);
                                                if(compsearcher != components.end())
                                                {
                                                    std::vector<std::string> args = std::vector<std::string>(words.begin() + 4, words.end());

                                                    Component* comp = compsearcher->second->clone();
                                                    comp->construct(args);
                                                    searcher->second->addComponent(comp);
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        Logger() << "Entity " << number << " does not exist" << std::endl;
                                    }
                                }
                            }
                        }
                    }
                }
                else if(words[0] == "move")
                {
                    if(words.size() >= 2)
                    {
                        int number = stringToInt(words[1]);
                        if(number != -9999)
                        {
                            std::unordered_map<EntityID, Entity*>::iterator searcher = entities.find(number);
                            if(searcher != entities.end())
                            {
                                if(searcher->second->hasComponent(WorldComponent::getStaticID()))
                                {
                                    WorldComponent* worldComp = static_cast<WorldComponent*>(searcher->second->getComponent(WorldComponent::getStaticID()));

                                    if(words.size() >= 5)
                                    {
                                        int x = stringToFloat(words[2]);
                                        int y = stringToFloat(words[3]);
                                        int z = stringToFloat(words[4]);

                                        worldComp->offsetPosition += glm::vec3(x,y,z);
                                        worldComp->updateData();
                                    }
                                }
                            }
                            else
                            {
                                Logger() << "Entity " << number << " does not exist" << std::endl;
                            }
                        }
                    }
                }
                else if(words[0] == "output")
                {
                    if(words.size() >= 2)
                    {
                        if(words[1] == "fps")
                        {
                            if(words.size() >= 3)
                            {
                                if(words[2] == "toggle")
                                {
                                    outputFPS = !outputFPS;
                                }
                                else if(words[2] == "true")
                                {
                                    outputFPS = true;
                                }
                                else if(words[2] == "false")
                                {
                                    outputFPS = false;
                                }
                            }
                        }
                        else if(words[1] == "subscription")
                        {
                            if(words.size() >= 3)
                            {
                                if(words[2] == "toggle")
                                {
                                    outputSub = !outputSub;
                                }
                                else if(words[2] == "true")
                                {
                                    outputSub = true;
                                }
                                else if(words[2] == "false")
                                {
                                    outputSub = false;
                                }
                            }
                        }
                        else if(words[1] == "deletion")
                        {
                            if(words.size() >= 3)
                            {
                                if(words[2] == "toggle")
                                {
                                    outputDel = !outputDel;
                                }
                                else if(words[2] == "true")
                                {
                                    outputDel = true;
                                }
                                else if(words[2] == "false")
                                {
                                    outputDel = false;
                                }
                            }
                        }
                    }
                }
            }
        }
        //SHOULD NOT BE HERE
        //Here to prevent insanely high delta when
        //exiting console
        lastFrame = glfwGetTime();
    }*/
}

void listComponents(EntityID inEntityID)
{
    Entity* inEntity = entities[inEntityID];
    for(std::unordered_map<ComponentID, Component*>::iterator componentPair = inEntity->components.begin(); componentPair != inEntity->components.end(); ++componentPair)
    {
        Logger() << "    " << componentPair->first << " - " << componentPair->second->vanityName << std::endl;
    }
    Logger() << "  Total:" << inEntity->components.size() << std::endl;
}
