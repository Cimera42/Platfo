#ifndef RENDERSYSTEM2D_H_INCLUDED
#define RENDERSYSTEM2D_H_INCLUDED

#include "system.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

class Render2DSystem : public System
{
    private:
        static SystemID ID;

    public:
        Render2DSystem();
        virtual ~Render2DSystem();

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

#endif // RENDERSYSTEM2D_H_INCLUDED
