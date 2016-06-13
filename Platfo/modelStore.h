#ifndef MODELSTORE_H_INCLUDED
#define MODELSTORE_H_INCLUDED

#include "store.h"
#include "mesh.h"

class ModelStore : public Store
{
public:
    ModelStore();
    ~ModelStore();
    void loadStore(Json::Value inValue);

    std::string modelFile;

    //Loaded
    Mesh mesh;
};

#endif // MODELSTORE_H_INCLUDED
