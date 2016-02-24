#ifndef MODELSTORE_H_INCLUDED
#define MODELSTORE_H_INCLUDED

#include "fileReader.h"
#include "store.h"
#include "openGLFunctions.h"
#include <pthread.h>
#include "mesh.h"
#include "textureStore.h"

class ModelStore : public Store
{
public:
    ModelStore();
    ~ModelStore();
    void loadStore(std::string);

    //Extracted
    DataBlock* modelBlock;
    std::string modelFile;

    //Loaded
    Mesh mesh;

    pthread_mutex_t modelLoadMutex;
};

#endif // MODELSTORE_H_INCLUDED
