#include "textureStore.h"
#include "loadTexture.h"
#include "logger.h"
#include "globals.h"

///TextureStore allows us to store the actual textures from files!
TextureStore::TextureStore()
{
    //This store deals with only extracting 1 texture from 1 file.
    /* TODO:
    - multiple textures - texturepackStore? - this will evoke many textureStores which can be used to preload data (or pass pointers around I guess)
    - fix this up to only contain 1 texture & make property loading less dodgy
    */

    //Default values
    textureLoadMutex = PTHREAD_MUTEX_INITIALIZER;
}

TextureStore::~TextureStore()
{
    for(int i = 0; i < textureList.size(); i++)
    {
        glDeleteTextures(1, &textureList[i].textureID);
    }
}

void TextureStore::loadStore(std::string name)
{
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    pthread_mutex_lock(&context_lock);
    GLFWwindow* tempWindow = glfwCreateWindow(1,1,"",NULL,glContext);
    pthread_mutex_unlock(&context_lock);
    if(MULTITHREADED_LOADING)
        glfwMakeContextCurrent(tempWindow);

    //Read file
    File readFile;
    textureBlock = readFile.readFromFile(name);
    if(readFile.success)
    {
        while(textureBlock->getNextElement()) //Changed to if for only the one texture.
        {
            if(textureBlock->checkCurrentElement("Texture2D"))
            {
                TextureData textureData;

                while(textureBlock->getNextProperty())
                {
                    if(textureBlock->checkCurrentProperty("filename"))
                        textureData.textureFile = readFile.fileDirectory+textureBlock->getCurrentValue<std::string>(0);
                    else if(textureBlock->checkCurrentProperty("srgb"))
                        textureData.srgb = textureBlock->getCurrentValue<bool>(0);
                    else
                        Logger()<<"Innapropriate texture property in: "<<readFile.fileName<<std::endl;
                }
                if (textureData.textureFile != "") //After we've loaded all properties for the texture element, we can actually load the texture!...
                {
                    int tempID = load2DTexture(textureData.textureFile, textureData.srgb); //Load the actual texture we store
                    if(tempID != 0)
                    {
                        pthread_mutex_lock(&textureLoadMutex);
                        textureData.textureID = tempID;
                        pthread_mutex_unlock(&textureLoadMutex);
                    }
                }
                textureList.push_back(textureData);
            }
            else if(textureBlock->checkCurrentElement("TextureCubemap"))
            {
                TextureData textureData;
                std::vector<std::string> fileNames;
                while(textureBlock->getNextProperty())
                {
                    if(textureBlock->checkCurrentProperty("filename"))
                    {
                        for(int i=0; i<6; i++) //Load each face of the cubemap
                        {
                            fileNames.push_back(readFile.fileDirectory+textureBlock->getCurrentValue<std::string>(i));
                        }
                    }
                    else
                        Logger()<<"Innapropriate texture property in: "<<readFile.fileName<<std::endl;
                }
                if (fileNames.size() != 0) //After we've loaded all properties for the texture element, we can actually load the texture!...
                {
                    int tempID = loadCubemapTexture(fileNames); //Load the actual texture we store
                    if(tempID != 0)
                    {
                        pthread_mutex_lock(&textureLoadMutex);
                        textureData.textureID = tempID;
                        pthread_mutex_unlock(&textureLoadMutex);
                    }
                }
                textureList.push_back(textureData);
            }
        }
        correctlyLoaded = true;
    }
    glfwDestroyWindow(tempWindow);
}
