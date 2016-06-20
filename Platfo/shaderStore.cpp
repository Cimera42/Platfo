#include "shaderStore.h"
#include <exception>
#include "fileReader.h"
#include "loadShader.h"
#include "globals.h"

//ShaderStore allows us to store the actual shader from files!
ShaderStore::ShaderStore()
{
    //Default values
    shaderID = 0;
    vertFile = "";
    fragFile = "";
}

ShaderStore::~ShaderStore()
{
    glDeleteShader(shaderID);
}

void ShaderStore::loadStore(Json::Value inValue)
{
    try
    {
        vertFile = inValue["vertexFile"].asString();
        fragFile = inValue["fragmentFile"].asString();
        shaderID = loadShader(vertFile.c_str(), fragFile.c_str());
        if(shaderID == 0)
        {
            throw;
        }
        correctlyLoaded = true;
    }
    catch(std::exception& e)
    {
        Logger()<<"Shader "<<vertFile<<", "<<fragFile<<" failed to load. "<<e.what()<<std::endl;
    }
}
