#ifndef WINDOWCOMPONENT_H_INCLUDED
#define WINDOWCOMPONENT_H_INCLUDED

#include "component.h"
#include <glm/glm.hpp>
#include "openGLFunctions.h"
#include <string>


class WindowComponent : public Component
{
    private:
        static ComponentID ID;

    public:
        WindowComponent();
        virtual ~WindowComponent();
        WindowComponent* construct(Json::Value inValue);
        WindowComponent* clone() {return new WindowComponent(*this);}

        //Values set
        std::string windowTitle;
        glm::vec2 windowSize;
        bool modeWindowed = true;
        bool modeFullscreen = false;
        bool modeBorderless = false;

        //Values generated
        float windowAspect;
        int clientWidth, clientHeight;
        GLFWwindow* glfwWindow;
        GLFWmonitor* glfwMonitor;
        const GLFWvidmode* glfwVideoMode;

        //Auto generation of ID
        ComponentID getID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
        static ComponentID getStaticID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
};

void windowCloseEvent(GLFWwindow*);

#endif // WINDOWCOMPONENT_H_INCLUDED
