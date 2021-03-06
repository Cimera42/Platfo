#include "terrainSystem.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "worldComponent.h"
#include "terrainComponent.h"
#include "render3DSystem.h"
#include "camera3DComponent.h"
#include "camera3DSystem.h"
#include "windowComponent.h"
#include "render2DSystem.h"
#include "render2DComponent.h"

SystemID TerrainSystem::ID;

TerrainSystem::TerrainSystem()
{
    std::vector<ComponentID> subList1;
    //Components needed to subscribe to system
    subList1.push_back(WorldComponent::getStaticID());
    subList1.push_back(TerrainComponent::getStaticID());
    addSubList(subList1);
}
TerrainSystem::~TerrainSystem(){}

void TerrainSystem::update()
{
    //Camera operations
    glm::mat4 cameraViewMatrix;
    glm::mat4 cameraProjMatrix;
    Camera3DSystem* cameraSys = static_cast<Camera3DSystem*>(systems[Camera3DSystem::getStaticID()]);
    if(cameraSys->activeCamera != -1)
    {
        Camera3DComponent* cameraComp = static_cast<Camera3DComponent*>(entities[cameraSys->activeCamera]->getComponent(Camera3DComponent::getStaticID()));
        //Send view and projection matrix to shader

        cameraViewMatrix = cameraComp->viewMatrix;
        cameraProjMatrix = cameraComp->projectionMatrix;
    }

    Render3DSystem* renderSys = static_cast<Render3DSystem*>(systems[Render3DSystem::getStaticID()]);
    glSetBindFramebuffer(GL_FRAMEBUFFER, renderSys->framebufferID);

    //glClearColor(0.0f,0.0f,0.0f,0.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(int subID = 0; subID < subscribedEntities[0].size(); subID++)
    {
        Entity* entity = entities[subscribedEntities[0][subID]];

        WorldComponent* worldComp = static_cast<WorldComponent*>(entity->getComponent(WorldComponent::getStaticID()));
        TerrainComponent* renderComp = static_cast<TerrainComponent*>(entity->getComponent(TerrainComponent::getStaticID()));

        if(!renderComp->modelStore->mesh.VAO)
            renderComp->modelStore->mesh.createVAO();

        if(renderComp->shaderStore->shaderID)
        {
            if(renderComp->viewMatLoc == -1)
                renderComp->findShaderLocations();

            glSetUseProgram(renderComp->shaderStore->shaderID);
            glUniformMatrix4fv(renderComp->viewMatLoc, 1, GL_FALSE, &cameraViewMatrix[0][0]);
            glUniformMatrix4fv(renderComp->projMatLoc, 1, GL_FALSE, &cameraProjMatrix[0][0]);
            glUniformMatrix4fv(renderComp->modelMatLoc, 1, GL_FALSE, &worldComp->modelMatrix[0][0]);

            //Bind texture
            if(renderComp->textureStore->correctlyLoaded)
            {
                glSetActiveTexture(GL_TEXTURE0);
                glSetBindTexture(GL_TEXTURE_2D, renderComp->textureStore->textureID);
                glUniform1i(renderComp->textureLoc, 0);
            }

            //Draw
            glSetBindVertexArray(renderComp->modelStore->mesh.VAO);
                glDrawElements(GL_TRIANGLES, renderComp->modelStore->mesh.bufSize, GL_UNSIGNED_INT, 0);
            glSetBindVertexArray(0);
        }
    }
    glSetBindFramebuffer(GL_FRAMEBUFFER, 0);
}
