#ifndef RENDERSKYBOXCOMPONENT_H_INCLUDED
#define RENDERSKYBOXCOMPONENT_H_INCLUDED

#include "component.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
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
        RenderSkyboxComponent* construct(std::string,std::string,std::string);
        RenderSkyboxComponent* construct(std::vector<std::string>);
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
