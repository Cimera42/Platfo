#include "sceneStore.h"
#include "fileReader.h"
#include "globals.h"
#include "openGLFunctions.h"

#include "main.h"

#include "windowComponent.h"
#include "renderScreenComponent.h"
#include "render2DComponent.h"
#include "render3DComponent.h"
#include "renderSkyboxComponent.h"
#include "worldComponent.h"
#include "tempplayerControlComponent.h"
#include "camera2DComponent.h"
#include "camera3DComponent.h"
#include "physicsComponent.h"
#include "playerComponent.h"
#include "directionalLightComponent.h"
#include "pointLightComponent.h"
#include "spotLightComponent.h"
#include "mouseRotationComponent.h"
#include "terrainComponent.h"

/*SceneStore allows us to store the entities and any global properties.
    - Evokes components on entities. Essentially the only reason this is a store is to allow for preloading of levels in the future.
*/
SceneStore::SceneStore(){
    sceneFile = "";
}

void SceneStore::loadStore(Json::Value inValue)
{
    try
    {
        JSONFile parsedScene;
        sceneFile = inValue["sceneFile"].asString();
        bool readSuccess = parsedScene.readFile(sceneFile);
        if(readSuccess)
        {
            Json::Value sceneValue = parsedScene.getValue();
            Json::Value entitiesValue = sceneValue["Entities"];
            for(int i = 0; i < entitiesValue.size(); i++)
            {
                Json::Value entityValue = entitiesValue[i];
                Entity * ent = new Entity();
                addEntity(ent);
                ent->vanityName = entityValue["Name"].asString();

                Json::Value componentValue = entityValue["Components"];
                if(componentValue.isMember("window"))
                {
                    WindowComponent* win = (new WindowComponent())->construct(componentValue["window"]);
                    delete mainWindow;
                    mainWindow = win;
                    ent->addComponent(win);
                }
                if(componentValue.isMember("renderScreen"))
                {
                    RenderScreenComponent* render = (new RenderScreenComponent())->construct(componentValue["renderScreen"]);
                    ent->addComponent(render);
                }
                if(componentValue.isMember("render2d"))
                {
                    Render2DComponent* render = (new Render2DComponent())->construct(componentValue["render2d"]);
                    ent->addComponent(render);
                }
                if(componentValue.isMember("render3d"))
                {
                    Render3DComponent* render = (new Render3DComponent())->construct(componentValue["render3d"]);
                    ent->addComponent(render);
                }
                if(componentValue.isMember("terrain"))
                {
                    TerrainComponent* terrain = (new TerrainComponent())->construct(componentValue["terrain"]);
                    ent->addComponent(terrain);
                }
                if(componentValue.isMember("renderSkybox"))
                {
                    RenderSkyboxComponent* render = (new RenderSkyboxComponent())->construct(componentValue["renderSkybox"]);
                    ent->addComponent(render);
                }
                if(componentValue.isMember("world"))
                {
                    WorldComponent* world = (new WorldComponent())->construct(componentValue["world"]);
                    ent->addComponent(world);
                }
                if(componentValue.isMember("player"))
                {
                    PlayerComponent* player = (new PlayerComponent())->construct(componentValue["player"]);
                    ent->addComponent(player);
                }
                if(componentValue.isMember("physics"))
                {
                    PhysicsComponent* physics = (new PhysicsComponent())->construct(componentValue["physics"]);
                    ent->addComponent(physics);
                }
                if(componentValue.isMember("camera2d"))
                {
                    Camera2DComponent* camera = (new Camera2DComponent())->construct(componentValue["camera2d"]);
                    ent->addComponent(camera);
                }
                if(componentValue.isMember("camera3d"))
                {
                    Camera3DComponent* camera = (new Camera3DComponent())->construct(componentValue["camera3d"]);
                    ent->addComponent(camera);
                }
                if(componentValue.isMember("control"))
                {
                    PlayerControlComponent* control = (new PlayerControlComponent())->construct(componentValue["control"]);
                    ent->addComponent(control);
                }
                if(componentValue.isMember("directionalLight"))
                {
                    DirectionalLightComponent* directionalLight = (new DirectionalLightComponent())->construct(componentValue["directionalLight"]);
                    ent->addComponent(directionalLight);
                }
                if(componentValue.isMember("pointLight"))
                {
                    PointLightComponent* pointLight = (new PointLightComponent())->construct(componentValue["pointLight"]);
                    ent->addComponent(pointLight);
                }
                if(componentValue.isMember("spotLight"))
                {
                    SpotLightComponent* spotLight = (new SpotLightComponent())->construct(componentValue["spotLight"]);
                    ent->addComponent(spotLight);
                }
                if(componentValue.isMember("mouseRot"))
                {
                    MouseRotationComponent* mouseRotComp = (new MouseRotationComponent())->construct(componentValue["mouseRot"]);
                    ent->addComponent(mouseRotComp);
                }
                if(componentValue.isMember("childOf"))
                {
                    /*std::string entityName = sceneBlock->getCurrentValue<std::string>(0);
                    for(std::unordered_map<EntityID, Entity*>::iterator entityPair = entities.begin(); entityPair != entities.end(); ++entityPair)
                    {
                        Entity* checkEnt = entityPair->second;
                        if(checkEnt->vanityName.compare(entityName) == 0)
                        {
                            checkEnt->addChild(ent);
                        }
                    }*/
                }
            }
            correctlyLoaded = true;
        }
    }
    catch(std::exception& e)
    {
        Logger()<<"Scene "<<sceneFile<<" failed to load. "<<e.what()<<std::endl;
    }
}
