#ifndef DIRECTIONALLIGHTCOMPONENT_H_INCLUDED
#define DIRECTIONALLIGHTCOMPONENT_H_INCLUDED

#include "component.h"
#include <glm/glm.hpp>

class DirectionalLightComponent : public Component
{
    private:
        static ComponentID ID;

    public:
        DirectionalLightComponent();
        virtual ~DirectionalLightComponent();
        DirectionalLightComponent* construct(float, glm::vec3);
        DirectionalLightComponent* construct(std::vector<std::string>);
        DirectionalLightComponent* clone() {return new DirectionalLightComponent(*this);}

        float intensity;
        glm::vec3 colour;

        //Auto generation of ID
        ComponentID getID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
        static ComponentID getStaticID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
};

#endif // DIRECTIONALLIGHTCOMPONENT_H_INCLUDED
