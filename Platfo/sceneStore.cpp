#include "sceneStore.h"

#include "openGLFunctions.h"
#include "entity.h"
#include "globals.h"
#include "main.h"
#include "windowComponent.h"
#include "render2DComponent.h"
#include "render3DComponent.h"
#include "worldComponent.h"
#include "tempplayerControlComponent.h"
#include "camera2DComponent.h"
#include "camera3DComponent.h"
#include "physicsComponent.h"
#include "playerComponent.h"
#include "logger.h"
/**SceneStore allows us to store the entities and any global properties.
    - Evokes components on entities. Essentially the only reason this is a store is to allow for preloading of levels in the future.
**/
SceneStore::SceneStore()
{

}

void SceneStore::loadStore(std::string name)
{
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    GLFWwindow* tempWindow = glfwCreateWindow(1,1,"",NULL,glContext);
    glfwMakeContextCurrent(tempWindow);
    //load the actual scene
    File readFile;
    sceneBlock = readFile.readFromFile(name);
    if(readFile.success)
    {
        //process this block
        while(sceneBlock->getNextElement()) //Changed to if for only the one texture.
        {
            if(sceneBlock->checkCurrentElement("Entity"))
            {
                Entity * ent = new Entity(); //Create the new entity from file
                addEntity(ent);
                ent->vanityName = sceneBlock->getCurrentIdentifier();
                while(sceneBlock->getNextProperty()) //Add the components to the entity
                {
                    if(sceneBlock->checkCurrentProperty("window"))
                    {
                        //Window component
                        /*std::string filename = readFile.fileDirectory+sceneBlock->getCurrentValue<std::string>(0);
                        WindowComponent* win = (new WindowComponent())->construct(filename);
                        //Temporary system init???? WHERE TO PUT
                        ent->addComponent(win);*/
                    }
                    else if(sceneBlock->checkCurrentProperty("render2d"))
                    {
                        //Render2D component FOR NOW
                        glm::vec4 uvs = sceneBlock->getCurrentValue<glm::vec4>(0);
                        Render2DComponent* render = (new Render2DComponent())->construct(glm::vec2(uvs.x,uvs.y), glm::vec2(uvs.z,uvs.w));
                        ent->addComponent(render);
                    }
                    else if(sceneBlock->checkCurrentProperty("render3d"))
                    {
                        //Render3D component FOR NOW
                        std::string modelPath = sceneBlock->getCurrentValue<std::string>(0);
                        std::string texturePath = sceneBlock->getCurrentValue<std::string>(1);
                        Render3DComponent* render = (new Render3DComponent())->construct(modelPath,texturePath);
                        ent->addComponent(render);
                    }
                    else if(sceneBlock->checkCurrentProperty("world"))
                    {
                        //World component FOR NOW
                        glm::vec3 position = sceneBlock->getCurrentValue<glm::vec3>(0);
                        glm::vec3 scale = sceneBlock->getCurrentValue<glm::vec3>(3);
                        glm::vec3 rotation = sceneBlock->getCurrentValue<glm::vec3>(6);
                        WorldComponent* world = (new WorldComponent())->construct(position, scale, rotation);
                        ent->addComponent(world);
                    }
                    else if(sceneBlock->checkCurrentProperty("player"))
                    {
                        //Player component FOR NOW
                        PlayerComponent* player = (new PlayerComponent())->construct(sceneBlock->getCurrentValue<int>(0));
                        ent->addComponent(player);
                    }
                    else if(sceneBlock->checkCurrentProperty("physics"))
                    {
                        //Physics component FOR NOW
                        PhysicsComponent* physics = (new PhysicsComponent())->construct(sceneBlock->getCurrentValue<float>(0),
                                                                                        sceneBlock->getCurrentValue<float>(1),
                                                                                        sceneBlock->getCurrentValue<float>(2));
                        ent->addComponent(physics);
                    }
                    else if(sceneBlock->checkCurrentProperty("camera2d"))
                    {
                        float zoom = sceneBlock->getCurrentValue<float>(0);
                        bool active = sceneBlock->getCurrentValue<bool>(1);
                        //Camera component FOR NOW
                        Camera2DComponent* camera = (new Camera2DComponent())->construct(zoom, active);
                        ent->addComponent(camera);
                    }
                    else if(sceneBlock->checkCurrentProperty("camera3d"))
                    {
                        float fov = sceneBlock->getCurrentValue<float>(0);
                        float nearDist = sceneBlock->getCurrentValue<float>(1);
                        float farDist = sceneBlock->getCurrentValue<float>(2);
                        bool active = sceneBlock->getCurrentValue<bool>(3);
                        //Camera component FOR NOW
                        Camera3DComponent* camera = (new Camera3DComponent())->construct(fov,nearDist,farDist, active);
                        ent->addComponent(camera);
                    }
                    else if(sceneBlock->checkCurrentProperty("control"))
                    {
                        float speed = sceneBlock->getCurrentValue<float>(0);
                        int forward = sceneBlock->getCurrentValue<char>(1);
                        int back = sceneBlock->getCurrentValue<char>(2);
                        int left = sceneBlock->getCurrentValue<char>(3);
                        int right = sceneBlock->getCurrentValue<char>(4);
                        int up = sceneBlock->getCurrentValue<char>(5);
                        int down = sceneBlock->getCurrentValue<char>(6);
                        //Player control component FOR NOW
                        PlayerControlComponent* control = (new PlayerControlComponent())->construct(speed, forward, back, left, right, up, down);
                        ent->addComponent(control);
                    }
                    else
                    {
                        Logger()<<"Innapropriate scene property in: "<<readFile.fileName<<std::endl;
                    }
                }
            }
        }
        correctlyLoaded = true;
    }
    glfwDestroyWindow(tempWindow);
}
