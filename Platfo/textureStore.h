#ifndef TEXTURESTORE_H_INCLUDED
#define TEXTURESTORE_H_INCLUDED

#include "fileReader.h"
#include "store.h"
#include "openGLFunctions.h"
#include <pthread.h>

struct TextureData
{
    std::string textureFile = "";
    bool srgb = false;

    //Loaded
    GLuint textureID = 0;
};

class TextureStore : public Store
{
public:
    TextureStore();
    ~TextureStore();
    void loadStore(std::string);

    //Extracted
    DataBlock* textureBlock;

    std::vector<TextureData> textureList;

    pthread_mutex_t textureLoadMutex;
};

#endif // TEXTURESTORE_H_INCLUDED
