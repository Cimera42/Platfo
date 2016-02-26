#ifndef SHADERSTORE_H_INCLUDED
#define SHADERSTORE_H_INCLUDED

#include "fileReader.h"
#include "store.h"
#include "openGLFunctions.h"
#include <pthread.h>

class ShaderStore : public Store
{
public:
    ShaderStore();
    ~ShaderStore();
    void loadStore(std::string);

    //Extracted
    DataBlock* shaderBlock;
    std::string vertFile;
    std::string fragFile;

    //Loaded
    GLuint shaderID;

    pthread_mutex_t shaderLoadMutex;
};

#endif // SHADERSTORE_H_INCLUDED
