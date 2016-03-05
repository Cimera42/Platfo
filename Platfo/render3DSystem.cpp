#include "render3DSystem.h"
#include "globals.h"

#include <iostream>
#include "worldComponent.h"
#include "render3DComponent.h"
#include "textureStore.h"
#include "camera3DComponent.h"
#include "camera3DSystem.h"
#include "loadShader.h"
#include "openGLFunctions.h"
#include "loader.h"
#include "own_funcs.h"
#include "windowComponent.h"
#include "render2DSystem.h"
#include "typeConversion.h"
#include "render2DComponent.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

SystemID Render3DSystem::ID;

Render3DSystem::Render3DSystem()
{
    std::vector<ComponentID> subList1;
    //Components needed to subscribe to system
    subList1.push_back(WorldComponent::getStaticID());
    subList1.push_back(Render3DComponent::getStaticID());
    addSubList(subList1);

    //Generate framebuffer and bind it
    glGenFramebuffers(1, &framebufferID);
    glSetBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    WindowComponent* window = mainWindow;
    int width = window->windowSize.x;
    int height = window->windowSize.y;

    GLuint renderBuffer;
    glGenRenderbuffers(1, &renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

    //List of texture ids
    std::vector<GLuint> textureList;
    //List of texture types for each texture
    std::vector<GLenum> textureAttachmentList;
    textureAttachmentList.push_back(GL_COLOR_ATTACHMENT0);
    textureAttachmentList.push_back(GL_COLOR_ATTACHMENT1);
    textureAttachmentList.push_back(GL_COLOR_ATTACHMENT2);
    textureAttachmentList.push_back(GL_DEPTH_ATTACHMENT);
    //List of render attachments for each texture, decided by the texture type
    std::vector<GLuint> fboAttachmentList;

    for(int i = 0; i < textureAttachmentList.size(); i++)
    {
        //Push empty variable to list so that textureID has something to generate into
        textureList.push_back(0);
        glGenTextures(1, &textureList[i]);
        glSetBindTexture(GL_TEXTURE_2D, textureList[i]);
        //Set
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        //Change the texture type depending on what it will contain
        switch(textureAttachmentList[i])
        {
            case GL_DEPTH_ATTACHMENT:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0); //Please change width and height!
                    fboAttachmentList.push_back(GL_NONE);
                break;

            default:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
                    fboAttachmentList.push_back(textureAttachmentList[i]);
                break;
        }

        //Tell FBO which texture to draw into
        glSetBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, textureAttachmentList[i], GL_TEXTURE_2D, textureList[i], 0);
    }

    //Tell the FBO what to draw to each texture
    glDrawBuffers(fboAttachmentList.size(),  &fboAttachmentList[0]);

    //Check if everything is OK
    GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (e != GL_FRAMEBUFFER_COMPLETE)
        std::cout<<"FBO - Framebuffer incomplete error: 3d fbo"<<std::endl;

    //Clean up
    std::cout<<fboAttachmentList.size()<<" colours attached to <3d fbo> with "<<textureAttachmentList.size()<<" total components. \n"<<std::endl;
    std::vector<GLuint>().swap(fboAttachmentList);

    Load<TextureStore>::Object(&textureStore, false, "3DRenderFBOTextures");
    for(int i = 0; i < textureList.size(); i++)
    {
        TextureData texData;
        texData.textureID = textureList[i];
        textureStore->textureList.push_back(texData);
    }
    textureStore->correctlyLoaded = true;
}
Render3DSystem::~Render3DSystem()
{
    Unload<TextureStore>::Object(&textureStore);
}

void Render3DSystem::update()
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

    glSetBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
    //glClearColor(0.0f,0.0f,0.0f,0.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(int subID = 0; subID < subscribedEntities[0].size(); subID++)
    {
        Entity* entity = entities[subscribedEntities[0][subID]];

        WorldComponent* worldComp = static_cast<WorldComponent*>(entity->getComponent(WorldComponent::getStaticID()));
        Render3DComponent* renderComp = static_cast<Render3DComponent*>(entity->getComponent(Render3DComponent::getStaticID()));

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
                glSetBindTexture(GL_TEXTURE_2D, renderComp->textureStore->textureList[0].textureID);
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
