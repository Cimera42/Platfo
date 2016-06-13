#ifndef RENDERSKYBOXCOMPONENT_H_INCLUDED
#define RENDERSKYBOXCOMPONENT_H_INCLUDED

#include "component.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "modelStore.h"
#include "textureStore.h"
#include "shaderStore.h"

class RenderSkyboxComponent : public Component
{
    private:
        static ComponentID ID;

    public:
        RenderSkyboxComponent();
        virtual ~RenderSkyboxComponent();
        RenderSkyboxComponent* construct(Json::Value inValue);
        RenderSkyboxComponent* clone() {return new RenderSkyboxComponent(*this);}

        ModelStore* modelStore;
        TextureStore* textureStore;
        ShaderStore* shaderStore;

        GLuint viewMatLoc;
        GLuint projMatLoc;
        GLuint textureLoc;

        void createVAO();
        void findShaderLocations();

        //Auto generation of ID
        ComponentID getID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
        static ComponentID getStaticID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
};

#endif // RENDERSKYBOXCOMPONENT_H_INCLUDED
