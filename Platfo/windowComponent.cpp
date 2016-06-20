#include "windowComponent.h"
#include "keyboardHandler.h"
#include "mouseHandler.h"

ComponentID WindowComponent::ID;

WindowComponent::WindowComponent(){vanityName = "Window Component";}
WindowComponent::~WindowComponent()
{
    //glfwDestroyWindow(glfwWindow);
    //Logger() << "Window destroyed" << std::endl;
}

WindowComponent* WindowComponent::construct(Json::Value inValue)
{
    //JSON Input
    windowTitle = inValue["title"].asString();
    windowSize = glm::vec2(inValue["size"][0].asDouble(), inValue["size"][1].asDouble());
    modeWindowed = inValue["windowed"].asBool();
    modeFullscreen = inValue["fullscreen"].asBool();
    modeBorderless = inValue["borderless"].asBool();


    //Actual window setup
    windowAspect = windowSize.x/windowSize.y;

    glfwMonitor = glfwGetPrimaryMonitor();
    glfwVideoMode = glfwGetVideoMode(glfwMonitor);
    clientWidth = glfwVideoMode->width;
    clientHeight = glfwVideoMode->height;
    glfwWindowHint(GLFW_RED_BITS, glfwVideoMode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, glfwVideoMode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, glfwVideoMode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, glfwVideoMode->refreshRate);
    if (modeFullscreen)
    {
        glfwWindow = glfwCreateWindow(windowSize.x,windowSize.y, windowTitle.c_str(), glfwMonitor, NULL);
    }
    else if(modeBorderless)
    {
        glfwWindowHint(GLFW_DECORATED, GL_FALSE);

        glfwWindow = glfwCreateWindow(windowSize.x,windowSize.y, windowTitle.c_str(), NULL, NULL);
        glfwSetWindowPos(glfwWindow, clientWidth/2-windowSize.x/2, clientHeight/2-windowSize.y/2);
    }
    else if(modeWindowed)
    {
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindow = glfwCreateWindow(windowSize.x,windowSize.y, windowTitle.c_str(), NULL, NULL);
        glfwSetWindowPos(glfwWindow, clientWidth/2-windowSize.x/2, clientHeight/2-windowSize.y/2);
    }
    if(!glfwWindow)
    {
        Logger() << "Failed to create window" << std::endl;
        return this;
    }
    else
    {
        glfwSetWindowCloseCallback(glfwWindow, windowCloseEvent);
        glfwSetKeyCallback(glfwWindow, keyboardInput);
        glfwSetCursorPosCallback(glfwWindow, mouseMoveInput);
        glfwSetMouseButtonCallback(glfwWindow, mouseButtonInput);
    }
    return this;
}

void windowCloseEvent(GLFWwindow* closingWindow)
{
    shouldExit = true;
}
