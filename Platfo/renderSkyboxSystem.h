#ifndef RENDERSKYBOXSYSTEM_H_INCLUDED
#define RENDERSKYBOXSYSTEM_H_INCLUDED

#include "system.h"
#include <GL/glew.h>

class RenderSkyboxSystem : public System
{
    private:
        static SystemID ID;

    public:
        RenderSkyboxSystem();
        virtual ~RenderSkyboxSystem();

        //GLuint framebufferID;

        //Auto generation of ID
        SystemID getID() {if(ID == 0) {ID = systemIDIncrementor++;} return ID;}
        static SystemID getStaticID() {if(ID == 0) {ID = systemIDIncrementor++;} return ID;}

        void update();
};

#endif // RENDERSKYBOXSYSTEM_H_INCLUDED
