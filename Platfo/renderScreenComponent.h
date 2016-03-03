#ifndef RENDERSCREENCOMPONENT_H_INCLUDED
#define RENDERSCREENCOMPONENT_H_INCLUDED

#include "component.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "textureStore.h"
#include "shaderStore.h"

class RenderScreenComponent : public Component
{
    private:
        static ComponentID ID;

    public:
        RenderScreenComponent();
        virtual ~RenderScreenComponent();
        RenderScreenComponent* construct(std::string,bool,std::string);
        RenderScreenComponent* construct(std::vector<std::string>);
        RenderScreenComponent* clone() {return new RenderScreenComponent(*this);}

        TextureStore* textureStore;
        ShaderStore* shaderStore;

        GLuint directionalLightLoc_direction;
        GLuint directionalLightLoc_intensity;
        GLuint directionalLightLoc_colour;

        GLuint pointLightLoc_location;
        GLuint pointLightLoc_intensity;
        GLuint pointLightLoc_attenuation;
        GLuint pointLightLoc_colour;

        void findShaderLocations();

        //Auto generation of ID
        ComponentID getID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
        static ComponentID getStaticID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
};

#endif // RENDERSCREENCOMPONENT_H_INCLUDED
