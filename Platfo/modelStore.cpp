#include "modelStore.h"
#include "mesh.h"
#include "logger.h"
#include "globals.h"
#include <pthread.h>

///ModelStore allows us to store the actual models from files!
ModelStore::ModelStore()
{
    //Default values
    modelLoadMutex = PTHREAD_MUTEX_INITIALIZER;
    modelFile = "";
}

ModelStore::~ModelStore()
{
}

void ModelStore::loadStore(std::string name)
{
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    pthread_mutex_lock(&context_lock);
    GLFWwindow* tempWindow = glfwCreateWindow(1,1,"",NULL,glContext);
    pthread_mutex_unlock(&context_lock);
    if(MULTITHREADED_LOADING)
        glfwMakeContextCurrent(tempWindow);

    //Read file
    File readFile;
    modelBlock = readFile.readFromFile(name);
    if(readFile.success)
    {
        while(modelBlock->getNextElement()) //Changed to if for only the one texture.
        {
            if(modelBlock->checkCurrentElement("Mesh"))
            {
                while(modelBlock->getNextProperty())
                {
                    if(modelBlock->checkCurrentProperty("filename"))
                    {
                        modelFile = readFile.fileDirectory+modelBlock->getCurrentValue<std::string>(0);
                    }
                    else
                    {
                        Logger()<<"Innapropriate model property in: "<<readFile.fileName<<std::endl;
                    }
                }
                if (modelFile != "") //After we've loaded all properties for the texture element, we can actually load the texture!...
                {
                    pthread_mutex_lock(&modelLoadMutex);

                    mesh.loadModel(modelFile);

                    pthread_mutex_unlock(&modelLoadMutex);
                    correctlyLoaded = true;
                }
            }
        }
    }
    glfwDestroyWindow(tempWindow);
}
