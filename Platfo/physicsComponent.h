#ifndef PHYSICSCOMPONENT_H_INCLUDED
#define PHYSICSCOMPONENT_H_INCLUDED

#include "component.h"
#include <glm/glm.hpp>

class PhysicsComponent : public Component //Rename to physicsComponent
{
    private:
        static ComponentID ID;

    public:
        PhysicsComponent();
        virtual ~PhysicsComponent();
        PhysicsComponent* construct(float,float,float);
        PhysicsComponent* construct(std::vector<std::string>);
        PhysicsComponent* clone() {return new PhysicsComponent(*this);}

        bool staticObject; //not affected by forces

        float mass;
        float coefficientRestitution; //objects bounciness 0-1
        float coefficientDrag; //Objects air resistance -->0 is streamlined, cube is 1.05
        //float coefficientFriction; //

        //float area; ?? literally just size

        glm::vec3 force;//Used to work out acceleration - resultant at movementSystem
        glm::vec3 acceleration;
        glm::vec3 velocity;


        void impulse(glm::vec3 in);

        //Auto generation of ID
        ComponentID getID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
        static ComponentID getStaticID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
};

#endif // PHYSICSCOMPONENT_H_INCLUDED
