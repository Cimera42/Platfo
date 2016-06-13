#ifndef CAMERA3DCOMPONENT_H_INCLUDED
#define CAMERA3DCOMPONENT_H_INCLUDED

#include "component.h"
#include <glm/glm.hpp>

class Camera3DComponent : public Component
{
    private:
        static ComponentID ID;

    public:
        Camera3DComponent();
        virtual ~Camera3DComponent();
        Camera3DComponent* construct(Json::Value inValue);
        Camera3DComponent* clone() {return new Camera3DComponent(*this);}

        bool activeFlag;

        float fov;
        float nearDistance;
        float farDistance;
        glm::mat4 projectionMatrix;
        glm::mat4 viewMatrix;

        void updateMatrix();

        //Auto generation of ID
        ComponentID getID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
        static ComponentID getStaticID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
};

#endif // CAMERA3DCOMPONENT_H_INCLUDED
