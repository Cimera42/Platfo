#include "terrainComponent.h"
#include "loadingSystem.h"
#include <glm/gtc/noise.hpp>
#include "mesh.h"
#include "loadTexture.h"

ComponentID TerrainComponent::ID;

TerrainComponent::TerrainComponent(){vanityName = "Terrain Component";}
TerrainComponent::~TerrainComponent()
{
    LoadingSystem* loadingSys = static_cast<LoadingSystem*>(systems[LoadingSystem::getStaticID()]);
    loadingSys->unload(&textureStore);
    loadingSys->unload(&shaderStore);
    delete modelStore;
}

TerrainComponent* TerrainComponent::construct(Json::Value inValue)
{
    //Load<ModelStore>::Object(&modelStore, false, "TerrainMesh");
    LoadingSystem* loadingSys = static_cast<LoadingSystem*>(systems[LoadingSystem::getStaticID()]);
    modelStore = new ModelStore();
    loadingSys->load<TextureStore>(&textureStore, inValue["textureStore"][0]);
    loadingSys->load<ShaderStore>(&shaderStore, inValue["shaderStore"][0]);

    textureLoc = -1;
    modelMatLoc = -1;
    viewMatLoc = -1;
    projMatLoc = -1;

    std::string heightmapPath = inValue["heightmap"].asString();
    bool randMap = (heightmapPath == "randGen");
    unsigned char* loadedMapData;
    int loadedWidth = 0;
    int loadedHeight = 0;
    if(!randMap)
    {
        loadedMapData = load2DTextureData(heightmapPath, &loadedWidth, &loadedHeight);

        if(loadedWidth != loadedHeight)
        {
            Logger() << "Heightmap image is not square" << std::endl;
            return this;
        }
    }

    int size = 100;
    if(loadedWidth > 0)
        size = loadedWidth;
            Logger() << loadedWidth << " - " << loadedHeight << std::endl;
    std::vector<glm::vec3> vertexArray(size*size, glm::vec3(0,0,0));
    std::vector<glm::vec2> uvArray(size*size, glm::vec2(0,0));
    std::vector<glm::vec3> normalArray(size*size, glm::vec3(0,1,0));
    std::vector<unsigned int> indexArray;
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            vertexArray[i*size + j].x = i;
            vertexArray[i*size + j].z = j;
            if(randMap)
            {
                vertexArray[i*size + j].y = glm::simplex(glm::vec2((float)i/10,(float)j/10));
            }
            else
            {
                float sum = 0;
                sum += loadedMapData[(i*size + j)*4+0];
                sum += loadedMapData[(i*size + j)*4+1];
                sum += loadedMapData[(i*size + j)*4+2];
                vertexArray[i*size + j].y = sum/3/256.0f;
            }
            uvArray[i*size + j] = glm::vec2(j,-i)/float(size);
        }
    }
    for(int i = 0; i < size-1; i++)
    {
        for(int j = 0; j < size-1; j++)
        {
            glm::vec3 norm1 = computeNormal(vertexArray[(i+1)*size + (j+1)],
                                            vertexArray[(i+1)*size + j],
                                            vertexArray[i*size + j]);
            normalArray[(i+1)*size + (j+1)] += norm1;
            normalArray[(i+1)*size + j] += norm1;
            normalArray[i*size + j] += norm1;

            indexArray.push_back((i+1)*size + (j+1));
            indexArray.push_back((i+1)*size + j);
            indexArray.push_back(i*size + j);

            glm::vec3 norm2 = computeNormal(vertexArray[i*size + j],
                                            vertexArray[i*size + (j+1)],
                                            vertexArray[(i+1)*size + (j+1)]);
            normalArray[i*size + j] += norm2;
            normalArray[i*size + (j+1)] += norm2;
            normalArray[(i+1)*size + (j+1)] += norm2;

            indexArray.push_back(i*size + j);
            indexArray.push_back(i*size + (j+1));
            indexArray.push_back((i+1)*size + (j+1));
        }
    }
    for(int i = 0; i < normalArray.size(); i++)
    {
        normalArray[i] = glm::normalize(normalArray[i]);
    }

    modelStore->mesh.loadWithVectors(vertexArray, uvArray, normalArray, indexArray);

    return this;
}

void TerrainComponent::createVAO()
{
    modelStore->mesh.createVAO();
}

void TerrainComponent::findShaderLocations()
{
    textureLoc = glGetUniformLocation(shaderStore->shaderID, "textureSampler");
    modelMatLoc = glGetUniformLocation(shaderStore->shaderID, "modelMat");
    viewMatLoc = glGetUniformLocation(shaderStore->shaderID, "viewMat");
    projMatLoc = glGetUniformLocation(shaderStore->shaderID, "projMat");
}
