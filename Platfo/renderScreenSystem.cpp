#include "renderScreenSystem.h"
#include "globals.h"

#include <iostream>
#include "worldComponent.h"
#include "renderScreenComponent.h"
#include "loadShader.h"
#include "openGLFunctions.h"
#include "loader.h"

#include "lightingSystem.h"
#include "camera3DSystem.h"

#include <glm/glm.hpp>

SystemID RenderScreenSystem::ID;

RenderScreenSystem::RenderScreenSystem()
{
    std::vector<ComponentID> subList1;
    //Components needed to subscribe to system
    subList1.push_back(RenderScreenComponent::getStaticID());
    addSubList(subList1);

    //Standard quad vertices
    vertices.push_back(glm::vec2(-1.0f,1.0f));
    int tl = vertices.size()-1;
    vertices.push_back(glm::vec2(1.0f,1.0f));
    int tr = vertices.size()-1;
    vertices.push_back(glm::vec2(-1.0f,-1.0f));
    int bl = vertices.size()-1;
    vertices.push_back(glm::vec2(1.0f,-1.0f));
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
RenderScreenSystem::~RenderScreenSystem()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &uvBuffer);
    glDeleteBuffers(1, &indexBuffer);
}

void RenderScreenSystem::update()
{
    LightingSystem* lightingSystem = static_cast<LightingSystem*>(systems[LightingSystem::getStaticID()]);
    DirectionalLightGroup directionalLights = lightingSystem->compileDirectional();
    PointLightGroup pointLights = lightingSystem->compilePoint();
    SpotLightGroup spotLights = lightingSystem->compileSpot();

    glm::vec3 cameraPosition;
    Camera3DSystem* cameraSys = static_cast<Camera3DSystem*>(systems[Camera3DSystem::getStaticID()]);
    if(cameraSys->activeCamera != -1)
    {
        WorldComponent* worldComp = static_cast<WorldComponent*>(entities[cameraSys->activeCamera]->getComponent(WorldComponent::getStaticID()));
        //Send view and projection matrix to shader

        cameraPosition = worldComp->position;
    }

    glDisable(GL_DEPTH_TEST);
    for(int subID = 0; subID < subscribedEntities[0].size(); subID++)
    {
        Entity* entity = entities[subscribedEntities[0][subID]];

        WorldComponent* worldComp = static_cast<WorldComponent*>(entity->getComponent(WorldComponent::getStaticID()));
        RenderScreenComponent* renderComp = static_cast<RenderScreenComponent*>(entity->getComponent(RenderScreenComponent::getStaticID()));

        if(renderComp->shaderStore->shaderID)
        {
            if(renderComp->directionalLightLoc_direction == -1)
                renderComp->findShaderLocations();

            glSetUseProgram(renderComp->shaderStore->shaderID);

            glUniform3fv(renderComp->cameraPositionLoc, 1, &cameraPosition[0]);

            if(directionalLights.count > 0)
            {
                glUniform1i(renderComp->directionalLightLoc_count, directionalLights.count);

                glUniform3fv(renderComp->directionalLightLoc_direction, directionalLights.direction.size(), &directionalLights.direction[0][0]);
                glUniform1fv(renderComp->directionalLightLoc_intensity, directionalLights.intensity.size(), &directionalLights.intensity[0]);
                glUniform3fv(renderComp->directionalLightLoc_colour, directionalLights.colour.size(), &directionalLights.colour[0][0]);
            }

            if(pointLights.count > 0)
            {
                glUniform1i(renderComp->pointLightLoc_count, pointLights.count);

                glUniform3fv(renderComp->pointLightLoc_location, pointLights.location.size(), &pointLights.location[0][0]);
                glUniform1fv(renderComp->pointLightLoc_intensity, pointLights.intensity.size(), &pointLights.intensity[0]);
                glUniform1fv(renderComp->pointLightLoc_attenuation, pointLights.attenuation.size(), &pointLights.attenuation[0]);
                glUniform3fv(renderComp->pointLightLoc_colour, pointLights.colour.size(), &pointLights.colour[0][0]);
            }

            if(spotLights.count > 0)
            {
                glUniform1i(renderComp->spotLightLoc_count, spotLights.count);

                glUniform3fv(renderComp->spotLightLoc_location, spotLights.location.size(), &spotLights.location[0][0]);
                glUniform3fv(renderComp->spotLightLoc_direction, spotLights.direction.size(), &spotLights.direction[0][0]);
                glUniform1fv(renderComp->spotLightLoc_intensity, spotLights.intensity.size(), &spotLights.intensity[0]);
                glUniform1fv(renderComp->spotLightLoc_attenuation, spotLights.attenuation.size(), &spotLights.attenuation[0]);
                glUniform2fv(renderComp->spotLightLoc_angle, spotLights.angle.size(), &spotLights.angle[0][0]);
                glUniform3fv(renderComp->spotLightLoc_colour, spotLights.colour.size(), &spotLights.colour[0][0]);
            }

            //Bind texture
            if(renderComp->textureStore->correctlyLoaded)
            {
                for(int i = 0; i < renderComp->textureStore->textureList.size(); i++)
                {
                    glSetActiveTexture(GL_TEXTURE0 + i);
                    glSetBindTexture(GL_TEXTURE_2D, renderComp->textureStore->textureList[i].textureID);
                    std::string s = "textureSampler"+InttoStr(i+1);
                    GLuint texLoc = glGetUniformLocation(renderComp->shaderStore->shaderID, s.c_str());
                    glUniform1i(texLoc, i);
                }
            }

            //Draw
            glSetBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, bufSize, GL_UNSIGNED_INT, 0);
            glSetBindVertexArray(0);
        }
    }
    glEnable(GL_DEPTH_TEST);
}
