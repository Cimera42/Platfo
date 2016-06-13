#ifndef POINTLIGHTCOMPONENT_H_INCLUDED
#define POINTLIGHTCOMPONENT_H_INCLUDED

#include "component.h"
#include <glm/glm.hpp>

class PointLightComponent : public Component
{
    private:
        static ComponentID ID;

    public:
        PointLightComponent();
        virtual ~PointLightComponent();
        PointLightComponent* construct(Json::Value inValue);
        PointLightComponent* clone() {return new PointLightComponent(*this);}

        float intensity;
        float attenuation;
        glm::vec3 colour;

        //Auto generation of ID
        ComponentID getID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
        static ComponentID getStaticID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
};

#endif // POINTLIGHTCOMPONENT_H_INCLUDED
