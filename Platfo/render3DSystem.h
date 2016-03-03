#ifndef RENDER3DSYSTEM_H_INCLUDED
#define RENDER3DSYSTEM_H_INCLUDED

#include "system.h"
#include <GL/glew.h>
#include "textureStore.h"

class Render3DSystem : public System
{
    private:
        static SystemID ID;

    public:
        Render3DSystem();
        virtual ~Render3DSystem();

        GLuint framebufferID;
        TextureStore* textureStore;

        //Auto generation of ID
        SystemID getID() {if(ID == 0) {ID = systemIDIncrementor++;} return ID;}
        static SystemID getStaticID() {if(ID == 0) {ID = systemIDIncrementor++;} return ID;}

        void update();
};

#endif // RENDER3DSYSTEM_H_INCLUDED
