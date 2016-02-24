#include "motionComponent.h"

ComponentID MotionComponent::ID;

MotionComponent::MotionComponent(){vanityName = "Motion Component";}
MotionComponent::~MotionComponent(){}
MotionComponent* MotionComponent::construct(float inMass, float inDrag)
{
    drag = inDrag;
    mass = inMass;
    velocity = glm::vec2(0,0);
    acceleration = glm::vec2(0,0);

    return this;
}
MotionComponent* MotionComponent::construct(std::vector<std::string> inArgs)
{
    if(inArgs.size() == 2)
    {
        float m = stringToFloat(inArgs[0]);
        float d = stringToFloat(inArgs[1]);

        if(m != -9999 && d != -9999)
        {
            this->construct(m,d);
        }
        else
        {
            Logger() << "Invalid input to Motion Component creation" << std::endl;
        }
    }
    else
    {
        Logger() << "Invalid number of arguments to Motion Component creation" << std::endl;
    }

    return this;
}

void MotionComponent::impulse(glm::vec2 in)
{
    velocity += in/mass; //delta v = impulse/mass
}
