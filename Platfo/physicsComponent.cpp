#include "physicsComponent.h"
#include "logger.h"
#include "typeConversion.h"

ComponentID PhysicsComponent::ID;

PhysicsComponent::PhysicsComponent(){vanityName = "Motion Component";}
PhysicsComponent::~PhysicsComponent(){}
PhysicsComponent* PhysicsComponent::construct(float inMass, float inRestitution, float inDrag)
{
    staticObject = false;
    mass = inMass;
    coefficientRestitution = inRestitution;
    coefficientDrag = inDrag;
    force = glm::vec3(0,0,0);
    velocity = glm::vec3(0,0,0);
    acceleration = glm::vec3(0,0,0);

    return this;
}
PhysicsComponent* PhysicsComponent::construct(std::vector<std::string> inArgs)
{
    if(inArgs.size() == 3)
    {
        float mas = stringToFloat(inArgs[0]);
        float res = stringToFloat(inArgs[1]);
        float dra = stringToFloat(inArgs[2]);

        if(mas != -9999 && res != -9999 &&
           dra != -9999)
        {
            this->construct(mas,res,dra);
        }
        else
        {
            Logger() << "Invalid input to Physics Component creation" << std::endl;
        }
    }
    else
    {
        Logger() << "Invalid number of arguments to Physics Component creation" << std::endl;
    }

    return this;
}

void PhysicsComponent::impulse(glm::vec3 in)
{
    velocity+= in/mass; //delta v = impulse/mass
}
