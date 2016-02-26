#include "camera3DComponent.h"
#include <glm/gtx/transform.hpp>
#include "typeConversion.h"
#include "logger.h"
#include "windowComponent.h"
#include "worldComponent.h"
#include "globals.h"

ComponentID Camera3DComponent::ID;

Camera3DComponent::Camera3DComponent(){vanityName = "Camera 3D Component";}
Camera3DComponent::~Camera3DComponent(){}
Camera3DComponent* Camera3DComponent::construct(float inFov, float inNear, float inFar, bool inActive)
{
    fov = inFov;
    nearDistance = inNear;
    farDistance = inFar;
    activeFlag = inActive;

    updateMatrix();
    return this;
}
Camera3DComponent* Camera3DComponent::construct(std::vector<std::string> inArgs)
{
    if(inArgs.size() == 2)
    {
        float fo = stringToFloat(inArgs[0]);
        float nea = stringToFloat(inArgs[1]);
        float fa = stringToFloat(inArgs[2]);
        bool act = stringToBool(inArgs[3]);

        if(fo != -9999 && nea != -9999 &&
           fa != -9999)
        {
            this->construct(fov,nea,fa,act);
        }
        else
        {
            Logger() << "Invalid input to Camera 3D Component creation" << std::endl;
        }
    }
    else
    {
        Logger() << "Invalid number of arguments to Camera 3D Component creation" << std::endl;
    }

    return this;
}

void Camera3DComponent::updateMatrix()
{
    WindowComponent* window = mainWindow;
    projectionMatrix = glm::perspective(fov, window->windowAspect, nearDistance,farDistance);

    if(parentEntity != -1)
    {
        if(entities[parentEntity]->hasComponent(WorldComponent::getStaticID()))
        {
            WorldComponent* worldComp = static_cast<WorldComponent*>(entities[parentEntity]->getComponent(WorldComponent::getStaticID()));

            viewMatrix = glm::lookAt(worldComp->position, worldComp->position+worldComp->forward, worldComp->up);
        }
    }
}
