#ifndef RENDER2DCOMPONENT_H_INCLUDED
#define RENDER2DCOMPONENT_H_INCLUDED

#include "component.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "textureStore.h"
#include "shaderStore.h"

class Render2DComponent : public Component
{
    private:
        static ComponentID ID;

    public:
        Render2DComponent();
        virtual ~Render2DComponent();
        Render2DComponent* construct(std::string,bool,std::string);
        Render2DComponent* construct(std::vector<std::string>);
        Render2DComponent* clone() {return new Render2DComponent(*this);}

        TextureStore* textureStore;
        ShaderStore* shaderStore;

        GLuint modelMatLoc;
        GLuint viewMatLoc;
        GLuint projMatLoc;

        void findShaderLocations();

        //Auto generation of ID
        ComponentID getID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
        static ComponentID getStaticID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
};

#endif // RENDER2DCOMPONENT_H_INCLUDED
