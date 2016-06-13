#ifndef TERRAINCOMPONENT_H_INCLUDED
#define TERRAINCOMPONENT_H_INCLUDED

#include "component.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "modelStore.h"
#include "textureStore.h"
#include "shaderStore.h"

class TerrainComponent : public Component
{
    private:
        static ComponentID ID;

    public:
        TerrainComponent();
        virtual ~TerrainComponent();
        TerrainComponent* construct(Json::Value inValue);
        TerrainComponent* clone() {return new TerrainComponent(*this);}

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

#endif // TERRAINCOMPONENT_H_INCLUDED
