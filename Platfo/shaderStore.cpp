#include "shaderStore.h"
#include "loadShader.h"
#include "logger.h"

///ShaderStore allows us to store the actual shader from files!
ShaderStore::ShaderStore()
{
    //Default values
    shaderLoadMutex = PTHREAD_MUTEX_INITIALIZER;
    shaderID = 0;
    vertFile = "";
    fragFile = "";
}

ShaderStore::~ShaderStore()
{
    glDeleteShader(shaderID);
}

void ShaderStore::loadStore(std::string name)
{
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    GLFWwindow* tempWindow = glfwCreateWindow(1,1,"",NULL,glContext);
    glfwMakeContextCurrent(tempWindow);

    //Read file
    File readFile;
    shaderBlock = readFile.readFromFile(name);
    if(readFile.success)
    {
        if(shaderBlock->getNextElement())
        {
            if(shaderBlock->checkCurrentElement("Shader"))
            {
                while(shaderBlock->getNextProperty())
                {
                    if(shaderBlock->checkCurrentProperty("vert"))
                        vertFile = readFile.fileDirectory+shaderBlock->getCurrentValue<std::string>(0);
                    else if(shaderBlock->checkCurrentProperty("frag"))
                        fragFile = readFile.fileDirectory+shaderBlock->getCurrentValue<std::string>(0);
                    else
                        Logger()<<"Innapropriate shader property in: "<<readFile.fileName<<std::endl;
                }
                if (vertFile != "" && fragFile != "") //After we've loaded all properties for the shader element, we can actually load the shader!...
                {
                    int tempID = loadShader(vertFile.c_str(), fragFile.c_str()); //Load the actual shader we store
                    if(tempID != 0)
                    {
                        pthread_mutex_lock(&shaderLoadMutex);
                        shaderID = tempID;
                        pthread_mutex_unlock(&shaderLoadMutex);

                        correctlyLoaded = true;
                    }
                }
            }
        }
    }
    glfwDestroyWindow(tempWindow);
}
