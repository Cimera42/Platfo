#ifndef MOUSEROTATIONSYSTEM_H_INCLUDED
#define MOUSEROTATIONSYSTEM_H_INCLUDED

#include "system.h"

class MouseRotationSystem : public System
{
    private:
        static SystemID ID;

    public:
        MouseRotationSystem();
        virtual ~MouseRotationSystem();

        //Auto generation of ID
        SystemID getID() {if(ID == 0) {ID = systemIDIncrementor++;} return ID;}
        static SystemID getStaticID() {if(ID == 0) {ID = systemIDIncrementor++;} return ID;}

        void update(float);
};

#endif // MOUSEROTATIONSYSTEM_H_INCLUDED
