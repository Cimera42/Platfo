#include "textureStore.h"
#include "globals.h"
#include "fileReader.h"
#include "loadTexture.h"

//TextureStore allows us to store the actual textures from files!
TextureStore::TextureStore()
{
    //Default values
    textureID = 0;
    textureFile = "";
    hasSRGB = false;
}

TextureStore::~TextureStore()
{
    glDeleteTextures(1, &textureID);
}

void TextureStore::loadStore(Json::Value inValue)
{
    try
    {
        if(inValue.isMember("cubeMapFile"))
        {
            std::vector<std::string> cubeMapPaths;
            cubeMapPaths.push_back(inValue["cubeMapFile"][0].asString());
            cubeMapPaths.push_back(inValue["cubeMapFile"][1].asString());
            cubeMapPaths.push_back(inValue["cubeMapFile"][2].asString());
            cubeMapPaths.push_back(inValue["cubeMapFile"][3].asString());
            cubeMapPaths.push_back(inValue["cubeMapFile"][4].asString());
            cubeMapPaths.push_back(inValue["cubeMapFile"][5].asString());
            textureFile = cubeMapPaths[0];
            textureID = loadCubemapTexture(cubeMapPaths);
            if(textureID == 0) {
                throw;
            }
        } else if(inValue.isMember("textureFile"))
        {
            textureFile = inValue["textureFile"].asString();
            hasSRGB = inValue["SRGB"].asBool();
            textureID = load2DTexture(textureFile,hasSRGB);
            if(textureID == 0) {
                throw;
            }
        } else{
            throw;
        }
    }
    catch(std::exception& e)
    {
        Logger()<<"Texture "<<textureFile<<" failed to load. "<<e.what()<<std::endl;
    }
}
