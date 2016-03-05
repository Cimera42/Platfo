#include "renderSkyboxSystem.h"
#include "globals.h"

#include <iostream>
#include "renderSkyboxComponent.h"
#include "render3DSystem.h"
#include "camera3DComponent.h"
#include "camera3DSystem.h"
#include "loadShader.h"
#include "openGLFunctions.h"
#include "loader.h"
#include "own_funcs.h"
#include "windowComponent.h"
#include "typeConversion.h"
#include "render2DComponent.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

SystemID RenderSkyboxSystem::ID;

RenderSkyboxSystem::RenderSkyboxSystem()
{
    std::vector<ComponentID> subList1;
    //Components needed to subscribe to system
    subList1.push_back(RenderSkyboxComponent::getStaticID());
    addSubList(subList1);
}
RenderSkyboxSystem::~RenderSkyboxSystem()
{
}

void RenderSkyboxSystem::update()
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

    //Replace with proper FBO store
    Render3DSystem* renderSys = static_cast<Render3DSystem*>(systems[Render3DSystem::getStaticID()]);
    glSetBindFramebuffer(GL_FRAMEBUFFER, renderSys->framebufferID);

    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(int subID = 0; subID < subscribedEntities[0].size(); subID++)
    {
        Entity* entity = entities[subscribedEntities[0][subID]];

        RenderSkyboxComponent* renderComp = static_cast<RenderSkyboxComponent*>(entity->getComponent(RenderSkyboxComponent::getStaticID()));

        if(!renderComp->modelStore->mesh.VAO)
            renderComp->modelStore->mesh.createVAO();

        if(renderComp->shaderStore->shaderID)
        {
            if(renderComp->viewMatLoc == -1)
                renderComp->findShaderLocations();

            glSetUseProgram(renderComp->shaderStore->shaderID);

            cameraViewMatrix[3][0] = 0;
            cameraViewMatrix[3][1] = 0;
            cameraViewMatrix[3][2] = 0;
            glUniformMatrix4fv(renderComp->viewMatLoc, 1, GL_FALSE, &cameraViewMatrix[0][0]);
            glUniformMatrix4fv(renderComp->projMatLoc, 1, GL_FALSE, &cameraProjMatrix[0][0]);

            //Bind texture
            if(renderComp->textureStore->correctlyLoaded)
            {
                glSetActiveTexture(GL_TEXTURE0);
                glSetBindTexture(GL_TEXTURE_CUBE_MAP, renderComp->textureStore->textureList[0].textureID);
                glUniform1i(renderComp->textureLoc, 0);
            }

            //Draw
            glDepthMask(GL_FALSE);
            glSetBindVertexArray(renderComp->modelStore->mesh.VAO);
                glDrawElements(GL_TRIANGLES, renderComp->modelStore->mesh.bufSize, GL_UNSIGNED_INT, 0);
            glSetBindVertexArray(0);
            glDepthMask (GL_TRUE);
        }
    }
    //glSetBindFramebuffer(GL_FRAMEBUFFER, 0);
}
