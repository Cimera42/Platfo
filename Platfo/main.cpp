#include "main.h"
#include "globals.h"
#include "entity.h"
#include "component.h"
#include "system.h"
#include "store.h"
#include "loader.h"

#include "openGLFunctions.h"

#include "windowSystem.h"
#include "render2DSystem.h"
#include "render3DSystem.h"
#include "renderSkyboxSystem.h"
#include "renderScreenSystem.h"
#include "sceneStore.h"
#include "textureStore.h"
#include "tempplayerControlSystem.h"
#include "camera2DSystem.h"
#include "camera3DSystem.h"
#include "mouseScreenCoordSystem.h"
#include "movementSystem.h"
#include "lightingSystem.h"
#include "mouseRotationSystem.h"
#include "consoleSystem.h"

#include "camera2DComponent.h"
#include "physicsComponent.h"
#include "camera3DComponent.h"
#include "render2DComponent.h"
#include "render3DComponent.h"
#include "renderSkyboxComponent.h"
#include "renderScreenComponent.h"
#include "tempplayerControlComponent.h"
#include "windowComponent.h"
#include "worldComponent.h"
#include "playerComponent.h"
#include "directionalLightComponent.h"
#include "pointLightComponent.h"
#include "spotLightComponent.h"
#include "mouseRotationComponent.h"

#include <iostream>
#include <string>
#include <fstream>

int main()
{
    initGLFW();
    initGLEW();

    //Temporary loading place for systems
    systems[WindowSystem::getStaticID()] = new WindowSystem();
    systems[Render3DSystem::getStaticID()] = new Render3DSystem();
    systems[RenderSkyboxSystem::getStaticID()] = new RenderSkyboxSystem();
    systems[RenderScreenSystem::getStaticID()] = new RenderScreenSystem();
    systems[Render2DSystem::getStaticID()] = new Render2DSystem();
    systems[PlayerControlSystem::getStaticID()] = new PlayerControlSystem();
    systems[Camera2DSystem::getStaticID()] = new Camera2DSystem();
    systems[Camera3DSystem::getStaticID()] = new Camera3DSystem();
    systems[MouseScreenCoordSystem::getStaticID()] = new MouseScreenCoordSystem();
    systems[MovementSystem::getStaticID()] = new MovementSystem();
    systems[LightingSystem::getStaticID()] = new LightingSystem();
    systems[MouseRotationSystem::getStaticID()] = new MouseRotationSystem();
    systems[ConsoleSystem::getStaticID()] = new ConsoleSystem();

    components[Camera2DComponent::getStaticID()] = new Camera2DComponent();
    components[Camera3DComponent::getStaticID()] = new Camera3DComponent();
    components[Render2DComponent::getStaticID()] = new Render2DComponent();
    components[Render3DComponent::getStaticID()] = new Render3DComponent();
    components[RenderSkyboxComponent::getStaticID()] = new RenderSkyboxComponent();
    components[RenderScreenComponent::getStaticID()] = new RenderScreenComponent();
    components[PlayerControlComponent::getStaticID()] = new PlayerControlComponent();
    components[WindowComponent::getStaticID()] = new WindowComponent();
    components[WorldComponent::getStaticID()] = new WorldComponent();
    components[PlayerComponent::getStaticID()] = new PlayerComponent();
    components[DirectionalLightComponent::getStaticID()] = new DirectionalLightComponent();
    components[PointLightComponent::getStaticID()] = new PointLightComponent();
    components[SpotLightComponent::getStaticID()] = new SpotLightComponent();
    components[MouseRotationComponent::getStaticID()] = new MouseRotationComponent();

    //File loading TEST
    SceneStore * scene;
    if(Load<SceneStore>::Object(&scene, true, "debug/scene.store"))
    {
        int fps = 0;
        double now = glfwGetTime();
        float delta = 0;
        lastFrame = glfwGetTime();

        glClearColor(0.55f,0.65f,0.8f,1.0f);
        while(!shouldExit)
        {
            delta = (glfwGetTime() - lastFrame);
            lastFrame = glfwGetTime();

            glClearColor(0.55f,0.65f,0.8f,1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //Temporary system loop
            //System to transform mouse movement to different spaces
            systems[MouseScreenCoordSystem::getStaticID()]->update();
            //Player movement system
            systems[PlayerControlSystem::getStaticID()]->update(delta);
            //Motion addition system
            systems[MovementSystem::getStaticID()]->update(delta);
            //Mouse rotation system
            systems[MouseRotationSystem::getStaticID()]->update(delta);
            //Camera 2D matrix calculation system
            systems[Camera2DSystem::getStaticID()]->update();
            //Camera 3D matrix calculation system
            systems[Camera3DSystem::getStaticID()]->update();
            //Render Skybox system
            systems[RenderSkyboxSystem::getStaticID()]->update();
            //3D rendering system
            systems[Render3DSystem::getStaticID()]->update();
            //Screen rendering system
            systems[RenderScreenSystem::getStaticID()]->update();
            //2D rendering system
            systems[Render2DSystem::getStaticID()]->update();
            //Should go last, since it updates window buffer
            systems[WindowSystem::getStaticID()]->update();
            //Command console
            systems[ConsoleSystem::getStaticID()]->update();

            //To be done per frame
            //to remove all entities
            //flagged for deletion
            deleteFlaggedEntities();

            glfwPollEvents();

            //Simple fps counter
            fps++;
            if(glfwGetTime() > now + 1.0f)
            {
                if(outputFPS)
                    Logger() << "Frametime:" << 1000.0f/fps << " FPS:" << fps << std::endl;
                now = glfwGetTime();
                fps = 0;
            }
        }

        deleteAllEntities();
        deleteAllSystems();
        delete mainWindow;
        glfwDestroyWindow(glContext);
        glfwTerminate();
    }
    return 0;
}
