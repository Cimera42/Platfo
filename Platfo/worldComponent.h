#ifndef WORLDCOMPONENT_H_INCLUDED
#define WORLDCOMPONENT_H_INCLUDED

#include "component.h"
#include <glm/glm.hpp>

class WorldComponent : public Component
{
    private:
        static ComponentID ID;

    public:
        WorldComponent();
        virtual ~WorldComponent();
        WorldComponent* construct(glm::vec3,glm::vec3,glm::vec3);
        WorldComponent* construct(std::vector<std::string>);
        WorldComponent* clone() {return new WorldComponent(*this);}

        glm::vec3 offsetPosition;

        glm::vec3 position;
        glm::vec3 scale;
        glm::vec3 rotation;

        glm::mat4 modelMatrix;

        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;

        void updateData();
        void updateMatrix();
        void updateChildren();

        WorldComponent* getParent();
        glm::vec3 getParentPosition();
        glm::vec3 getParentRotation();
        glm::mat4 getParentMatrix();

        //Auto generation of ID
        ComponentID getID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
        static ComponentID getStaticID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
};

#endif // WORLDCOMPONENT_H_INCLUDED
