#ifndef CAMERA3DSYSTEM_H_INCLUDED
#define CAMERA3DSYSTEM_H_INCLUDED

#include "system.h"

class Camera3DSystem : public System
{
    private:
        static SystemID ID;

    public:
        Camera3DSystem();
        virtual ~Camera3DSystem();

        EntityID activeCamera;


        //Auto generation of ID
        SystemID getID() {if(ID == 0) {ID = systemIDIncrementor++;} return ID;}
        static SystemID getStaticID() {if(ID == 0) {ID = systemIDIncrementor++;} return ID;}

        void entitySubscribed(Entity*,int);
        void update();

        void setActiveCamera(EntityID);
};

#endif // CAMERA3DSYSTEM_H_INCLUDED
