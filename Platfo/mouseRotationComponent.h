#ifndef MOUSEROTATIONCOMPONENT_H_INCLUDED
#define MOUSEROTATIONCOMPONENT_H_INCLUDED

#include "component.h"

class MouseRotationComponent : public Component
{
    private:
        static ComponentID ID;

    public:
        MouseRotationComponent();
        virtual ~MouseRotationComponent();
        MouseRotationComponent* construct(float,float,float,float);
        MouseRotationComponent* construct(std::vector<std::string>);
        MouseRotationComponent* clone() {return new MouseRotationComponent(*this);}

        float pitchMin;
        float pitchMax;

        float yawMin;
        float yawMax;

        //Auto generation of ID
        ComponentID getID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
        static ComponentID getStaticID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
};

#endif // MOUSEROTATIONCOMPONENT_H_INCLUDED
