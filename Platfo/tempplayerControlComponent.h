#ifndef TEMPPLAYERCONTROLCOMPONENT_H_INCLUDED
#define TEMPPLAYERCONTROLCOMPONENT_H_INCLUDED

#include "component.h"

class PlayerControlComponent : public Component
{
    private:
        static ComponentID ID;

    public:
        PlayerControlComponent();
        virtual ~PlayerControlComponent();
        PlayerControlComponent* construct(Json::Value inValue);
        PlayerControlComponent* clone() {return new PlayerControlComponent(*this);}

        float speed;
        int forwardKey;
        int backKey;
        int leftKey;
        int rightKey;
        int upKey;
        int downKey;

        //Auto generation of ID
        ComponentID getID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
        static ComponentID getStaticID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
};

#endif // TEMPPLAYERCONTROLCOMPONENT_H_INCLUDED
