#ifndef TERRAINSYSTEM_H_INCLUDED
#define TERRAINSYSTEM_H_INCLUDED

#include "system.h"
#include "openGLFunctions.h"
#include "textureStore.h"

class TerrainSystem : public System
{
    private:
        static SystemID ID;

    public:
        TerrainSystem();
        virtual ~TerrainSystem();

        GLuint framebufferID;

        //Auto generation of ID
        SystemID getID() {if(ID == 0) {ID = systemIDIncrementor++;} return ID;}
        static SystemID getStaticID() {if(ID == 0) {ID = systemIDIncrementor++;} return ID;}

        void update();
};

#endif // TERRAINSYSTEM_H_INCLUDED
