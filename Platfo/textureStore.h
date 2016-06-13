#ifndef TEXTURESTORE_H_INCLUDED
#define TEXTURESTORE_H_INCLUDED

#include "store.h"
#include "openGLFunctions.h"
#include <vector>

class TextureStore : public Store
{
public:
    TextureStore();
    ~TextureStore();
    void loadStore(Json::Value inValue);

    //In case of cubmap, set to the first texture's path
    std::string textureFile;
    bool hasSRGB;

    //Loaded
    GLuint textureID = 0;
};

#endif // TEXTURESTORE_H_INCLUDED
