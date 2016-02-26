#include "render2DSystem.h"
#include "globals.h"

#include <iostream>
#include "worldComponent.h"
#include "camera2DComponent.h"
#include "camera2DSystem.h"
#include "render2DComponent.h"
#include "loadShader.h"
#include "openGLFunctions.h"
#include "loader.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

SystemID Render2DSystem::ID;

Render2DSystem::Render2DSystem()
{
    std::vector<ComponentID> subList1;
    //Components needed to subscribe to system
    subList1.push_back(WorldComponent::getStaticID());
    subList1.push_back(Render2DComponent::getStaticID());
    addSubList(subList1);

    //Standard quad vertices
    vertices.push_back(glm::vec2(-0.5f,0.5f));
    int tl = vertices.size()-1;
    vertices.push_back(glm::vec2(0.5f,0.5f));
    int tr = vertices.size()-1;
    vertices.push_back(glm::vec2(-0.5f,-0.5f));
    int bl = vertices.size()-1;
    vertices.push_back(glm::vec2(0.5f,-0.5f));
    int br = vertices.size()-1;

    //Standard quad uvs
    uvs.push_back(glm::vec2(0,1));
    uvs.push_back(glm::vec2(1,1));
    uvs.push_back(glm::vec2(0,0));
    uvs.push_back(glm::vec2(1,0));

    //Standard quad indices
    indices.push_back(tl);
    indices.push_back(tr);
    indices.push_back(br);

    indices.push_back(tl);
    indices.push_back(bl);
    indices.push_back(br);

    //Generate vertex buffer using standard data
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);

    //Set up uv buffer for data stream
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

    //Generate index buffer using standard data
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    bufSize = indices.size();

    glGenVertexArrays(1, &VAO);
    glSetBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        //Enable vertex buffer
        int vertLoc = 0;
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glEnableVertexAttribArray(vertLoc);
        glVertexAttribPointer(vertLoc, 2, GL_FLOAT, GL_FALSE, 0, 0); //attribute, size, type, is normalised?, stride, offset

        int uvLoc = 1;
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glEnableVertexAttribArray(uvLoc);
        glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, 0, 0); //attribute, size, type, is normalised?, stride, offset
    glSetBindVertexArray(0);
}
Render2DSystem::~Render2DSystem()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &uvBuffer);
    glDeleteBuffers(1, &indexBuffer);
}

void Render2DSystem::update()
{
    //Camera operations
    glm::mat4 cameraViewMatrix;
    glm::mat4 cameraProjMatrix;
    Camera2DSystem* cameraSys = static_cast<Camera2DSystem*>(systems[Camera2DSystem::getStaticID()]);
    if(cameraSys->activeCamera != -1)
    {
        Camera2DComponent* cameraComp = static_cast<Camera2DComponent*>(entities[cameraSys->activeCamera]->getComponent(Camera2DComponent::getStaticID()));
        //Send view and projection matrix to shader

        cameraViewMatrix = cameraComp->viewMatrix;
        cameraProjMatrix = cameraComp->projectionMatrix;
    }

    glDisable(GL_DEPTH_TEST);
    for(int subID = 0; subID < subscribedEntities[0].size(); subID++)
    {
        Entity* entity = entities[subscribedEntities[0][subID]];

        WorldComponent* worldComp = static_cast<WorldComponent*>(entity->getComponent(WorldComponent::getStaticID()));
        Render2DComponent* renderComp = static_cast<Render2DComponent*>(entity->getComponent(Render2DComponent::getStaticID()));

        glSetUseProgram(renderComp->shader);
        glUniformMatrix4fv(renderComp->viewMatLoc, 1, GL_FALSE, &cameraViewMatrix[0][0]);
        glUniformMatrix4fv(renderComp->projMatLoc, 1, GL_FALSE, &cameraProjMatrix[0][0]);
        glUniformMatrix4fv(renderComp->modelMatLoc, 1, GL_FALSE, &worldComp->modelMatrix[0][0]);

        //Bind texture
        glSetActiveTexture(GL_TEXTURE0);
        glSetBindTexture(GL_TEXTURE_2D, renderComp->textureStore->textureID);
        glUniform1i(renderComp->textureLoc, 0);

        //Draw
        glSetBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, bufSize, GL_UNSIGNED_INT, 0);
        glSetBindVertexArray(0);
    }
    glEnable(GL_DEPTH_TEST);
}
