#include "terrainComponent.h"
#include "logger.h"
#include "typeConversion.h"
#include "loader.h"
#include "mesh.h"
#include "own_funcs.h"
#include "loadTexture.h"
#include <glm/gtc/noise.hpp>

ComponentID TerrainComponent::ID;

TerrainComponent::TerrainComponent(){vanityName = "Terrain Component";}
TerrainComponent::~TerrainComponent()
{
    Unload<ModelStore>::Object(&modelStore);
    Unload<TextureStore>::Object(&textureStore);
    Unload<ShaderStore>::Object(&shaderStore);
}

TerrainComponent* TerrainComponent::construct(std::string heightmapPath, std::string textureStorePath, std::string shaderStorePath)
{
    Load<ModelStore>::Object(&modelStore, false, "TerrainMesh");
    Load<TextureStore>::Object(&textureStore, true, textureStorePath);
    Load<ShaderStore>::Object(&shaderStore, true, shaderStorePath);

    textureLoc = -1;
    modelMatLoc = -1;
    viewMatLoc = -1;
    projMatLoc = -1;

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
TerrainComponent* TerrainComponent::construct(std::vector<std::string> inArgs)
{
    if(inArgs.size() == 0)
    {
        std::string mapPath = inArgs[0];
        std::string texturePath = inArgs[1];
        std::string shaderPath = inArgs[2];

        if(texturePath != "" && shaderPath != "")
            this->construct(mapPath, texturePath,shaderPath);
    }
    else
    {
        Logger() << "Invalid number of arguments to Terrain Component creation" << std::endl;
    }

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
