#ifndef CAMERA2DSYSTEM_H_INCLUDED
#define CAMERA2DSYSTEM_H_INCLUDED

#include "system.h"

class Camera2DSystem : public System
{
    private:
        static SystemID ID;

    public:
        Camera2DSystem();
        virtual ~Camera2DSystem();

        EntityID activeCamera;

        //Auto generation of ID
        SystemID getID() {if(ID == 0) {ID = systemIDIncrementor++;} return ID;}
        static SystemID getStaticID() {if(ID == 0) {ID = systemIDIncrementor++;} return ID;}

        void entitySubscribed(Entity*,int);
        void update();

        void setActiveCamera(EntityID);
};

#endif // CAMERA2DSYSTEM_H_INCLUDED
