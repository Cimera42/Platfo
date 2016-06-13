#include "camera3DComponent.h"
#include <glm/gtx/transform.hpp>
#include "windowComponent.h"
#include "worldComponent.h"

ComponentID Camera3DComponent::ID;

Camera3DComponent::Camera3DComponent(){vanityName = "Camera 3D Component";}
Camera3DComponent::~Camera3DComponent(){}
Camera3DComponent* Camera3DComponent::construct(Json::Value inValue)
{
    fov = inValue["fov"].asDouble();
    nearDistance = inValue["nearDistance"].asDouble();
    farDistance = inValue["farDistance"].asDouble();
    activeFlag = inValue["activeFlag"].asBool();

    updateMatrix();
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

            viewMatrix = glm::lookAt(worldComp->position, worldComp->position + worldComp->forward, worldComp->up);
        }
    }
}
