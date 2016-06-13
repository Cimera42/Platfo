#ifndef RENDERSCREENSYSTEM_H_INCLUDED
#define RENDERSCREENSYSTEM_H_INCLUDED

#include "system.h"
#include "openGLFunctions.h"
#include <glm/glm.hpp>

class RenderScreenSystem : public System
{
    private:
        static SystemID ID;

    public:
        RenderScreenSystem();
        virtual ~RenderScreenSystem();

        GLuint VAO;
        GLuint vertexBuffer;
        GLuint uvBuffer;
        GLuint indexBuffer;

        std::vector<glm::vec2> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<unsigned int> indices;
        int bufSize;

        //Auto generation of ID
        SystemID getID() {if(ID == 0) {ID = systemIDIncrementor++;} return ID;}
        static SystemID getStaticID() {if(ID == 0) {ID = systemIDIncrementor++;} return ID;}

        void update();
};

#endif // RENDERSCREENSYSTEM_H_INCLUDED
