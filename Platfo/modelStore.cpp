#include "modelStore.h"
#include <exception>
#include "globals.h"
#include "fileReader.h"
#include "mesh.h"

//ModelStore allows us to store the actual models from files!
ModelStore::ModelStore()
{
    //Default values
    modelFile = "";
}

ModelStore::~ModelStore(){}

void ModelStore::loadStore(Json::Value inValue)
{
    try
    {
        modelFile = inValue["modelFile"].asString();
        mesh.loadModel(modelFile);
        correctlyLoaded = true;
    }
    catch(std::exception& e)
    {
        Logger()<<"Model "<<modelFile<<" failed to load. "<<e.what()<<std::endl;
    }
}
