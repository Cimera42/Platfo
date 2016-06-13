#ifndef SHADERSTORE_H_INCLUDED
#define SHADERSTORE_H_INCLUDED

#include "store.h"
#include "openGLFunctions.h"

class ShaderStore : public Store
{
public:
    ShaderStore();
    ~ShaderStore();
    void loadStore(Json::Value inValue);

    std::string vertFile;
    std::string fragFile;

    //Loaded
    GLuint shaderID;
};

#endif // SHADERSTORE_H_INCLUDED
