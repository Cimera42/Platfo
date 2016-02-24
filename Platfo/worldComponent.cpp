#include "worldComponent.h"
#include <glm/gtx/transform.hpp>

#include "logger.h"
#include "typeConversion.h"
#include "own_funcs.h"

ComponentID WorldComponent::ID;

WorldComponent::WorldComponent(){vanityName = "World Component";}
WorldComponent::~WorldComponent(){}
WorldComponent* WorldComponent::construct(glm::vec3 inPosition, glm::vec3 inScale, glm::vec3 inRotation)
{
    position = inPosition;
    scale = inScale;
    rotation = glm::vec3(inRotation);

    updateMatrix();

    return this;
}
WorldComponent* WorldComponent::construct(std::vector<std::string> inArgs)
{
    if(inArgs.size() == 5)
    {
        float pX = stringToFloat(inArgs[0]);
        float pY = stringToFloat(inArgs[1]);
        float pZ = stringToFloat(inArgs[2]);

        float sX = stringToFloat(inArgs[3]);
        float sY = stringToFloat(inArgs[4]);
        float sZ = stringToFloat(inArgs[5]);

        float rotX = stringToFloat(inArgs[6]);
        float rotY = stringToFloat(inArgs[7]);
        float rotZ = stringToFloat(inArgs[8]);

        if(pX != -9999 && pY != -9999 && pZ != -9999 &&
           sX != -9999 && sY != -9999 && sZ != -9999 &&
           rotX != -9999 && rotY != -9999 && rotZ != -9999)
        {
            glm::vec3 pos = glm::vec3(pX,pY,pZ);
            glm::vec3 sca = glm::vec3(sX,sY,sZ);
            glm::vec3 rot = glm::vec3(rotX,rotY,rotZ);

            this->construct(pos,sca,rot);
        }
        else
        {
            Logger() << "Invalid input to World Component creation" << std::endl;
        }
    }
    else
    {
        Logger() << "Invalid number of arguments to World Component creation" << std::endl;
    }

    return this;
}

void WorldComponent::updateMatrix()
{
    modelMatrix = glm::mat4();
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1,0,0));
    modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0,1,0));
    modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0,0,1));
    modelMatrix = glm::scale(modelMatrix, scale);

    float pitch = toRad(rotation.x);
    float yaw = toRad(rotation.y);

    forward = glm::vec3(cos(pitch) * sin(yaw),
                             sin(pitch),
                             cos(pitch) * cos(yaw));
    right = glm::vec3(sin(yaw - 3.14f/2.0f),
                0,
                cos(yaw - 3.14f/2.0f));
    up = glm::cross(right, forward);
}
