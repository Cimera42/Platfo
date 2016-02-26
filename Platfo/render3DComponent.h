#ifndef RENDER3DCOMPONENT_H_INCLUDED
#define RENDER3DCOMPONENT_H_INCLUDED

#include "component.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include "modelStore.h"
#include "textureStore.h"
#include "shaderStore.h"

class Render3DComponent : public Component
{
    private:
        static ComponentID ID;

    public:
        Render3DComponent();
        virtual ~Render3DComponent();
        Render3DComponent* construct(std::string,std::string,std::string);
        Render3DComponent* construct(std::vector<std::string>);
        Render3DComponent* clone() {return new Render3DComponent(*this);}

        ModelStore* modelStore;
        TextureStore* textureStore;
        ShaderStore* shaderStore;

        GLuint textureLoc;
        GLuint viewMatLoc;
        GLuint projMatLoc;
        GLuint modelMatLoc;

        void createVAO();
        void findShaderLocations();

        //Auto generation of ID
        ComponentID getID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
        static ComponentID getStaticID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
};

#endif // RENDER3DCOMPONENT_H_INCLUDED
