#include "physicsComponent.h"

ComponentID PhysicsComponent::ID;

PhysicsComponent::PhysicsComponent(){vanityName = "Motion Component";}
PhysicsComponent::~PhysicsComponent(){}
PhysicsComponent* PhysicsComponent::construct(Json::Value inValue)
{
    isStatic = inValue["isStatic"].asBool();
    mass = inValue["mass"].asDouble();
    coefficientRestitution = inValue["restitution"].asDouble();
    coefficientDrag = inValue["drag"].asDouble();
    force = glm::vec3(0,0,0);
    velocity = glm::vec3(0,0,0);
    acceleration = glm::vec3(0,0,0);

    return this;
}

void PhysicsComponent::impulse(glm::vec3 in)
{
    velocity+= in/mass; //delta v = impulse/mass
}
