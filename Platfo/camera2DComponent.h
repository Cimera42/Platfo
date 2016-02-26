#ifndef CAMERA2DCOMPONENT_H_INCLUDED
#define CAMERA2DCOMPONENT_H_INCLUDED

#include "component.h"
#include <glm/glm.hpp>

class Camera2DComponent : public Component
{
    private:
        static ComponentID ID;

    public:
        Camera2DComponent();
        virtual ~Camera2DComponent();
        Camera2DComponent* construct(float inZoom, bool inActive);
        Camera2DComponent* construct(std::vector<std::string>);
        Camera2DComponent* clone() {return new Camera2DComponent(*this);}

        bool activeFlag;

        float zoom;
        glm::mat4 projectionMatrix;
        glm::mat4 viewMatrix;

        void updateMatrix();

        //Auto generation of ID
        ComponentID getID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
        static ComponentID getStaticID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
};

#endif // CAMERA2DCOMPONENT_H_INCLUDED
