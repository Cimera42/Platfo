#ifndef RENDERSYSTEM3D_H_INCLUDED
#define RENDERSYSTEM3D_H_INCLUDED

#include "system.h"
#include <GL/glew.h>

class Render3DSystem : public System
{
    private:
        static SystemID ID;

    public:
        Render3DSystem();
        virtual ~Render3DSystem();

        //Auto generation of ID
        SystemID getID() {if(ID == 0) {ID = systemIDIncrementor++;} return ID;}
        static SystemID getStaticID() {if(ID == 0) {ID = systemIDIncrementor++;} return ID;}

        void update();
};

#endif // RENDERSYSTEM3D_H_INCLUDED
