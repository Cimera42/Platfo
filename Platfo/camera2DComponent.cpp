#include "camera2DComponent.h"
#include <glm/gtx/transform.hpp>
#include "worldComponent.h"

ComponentID Camera2DComponent::ID;

Camera2DComponent::Camera2DComponent(){vanityName = "Camera 2D Component";}
Camera2DComponent::~Camera2DComponent(){}
Camera2DComponent* Camera2DComponent::construct(Json::Value inValue)
{
    zoom = inValue["zoom"].asDouble();
    activeFlag = inValue["activeFlag"].asBool();

    updateMatrix();
    return this;
}

void Camera2DComponent::updateMatrix()
{
    projectionMatrix = glm::ortho(-zoom, zoom, -zoom, zoom);

    if(parentEntity != -1)
    {
        if(entities[parentEntity]->hasComponent(WorldComponent::getStaticID()))
        {
            WorldComponent* worldComp = static_cast<WorldComponent*>(entities[parentEntity]->getComponent(WorldComponent::getStaticID()));

            viewMatrix = glm::lookAt(worldComp->position, worldComp->position+glm::vec3(0,0,1), glm::vec3(0,1,0));
        }
    }
}
