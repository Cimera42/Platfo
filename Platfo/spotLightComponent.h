#ifndef SPOTLIGHTCOMPONENT_H_INCLUDED
#define SPOTLIGHTCOMPONENT_H_INCLUDED

#include "component.h"
#include <glm/glm.hpp>

class SpotLightComponent : public Component
{
    private:
        static ComponentID ID;

    public:
        SpotLightComponent();
        virtual ~SpotLightComponent();
        SpotLightComponent* construct(Json::Value inValue);
        SpotLightComponent* clone() {return new SpotLightComponent(*this);}

        float intensity;
        float attenuation;
        glm::vec2 angle;
        glm::vec3 colour;

        //Auto generation of ID
        ComponentID getID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
        static ComponentID getStaticID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
};

#endif // SPOTLIGHTCOMPONENT_H_INCLUDED
