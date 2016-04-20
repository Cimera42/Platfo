#ifndef TERRAINCOMPONENT_H_INCLUDED
#define TERRAINCOMPONENT_H_INCLUDED

#include "component.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
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
        TerrainComponent* construct(std::string,std::string,std::string);
        TerrainComponent* construct(std::vector<std::string>);
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
