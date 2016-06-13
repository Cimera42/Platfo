#ifndef COMPONENT_H_INCLUDED
#define COMPONENT_H_INCLUDED

#include <string>
#include <vector>
#include <json/json.h>
#include "entity.h"
#include "globals.h"

typedef int ComponentID;

class Component
{
    private:
        static ComponentID ID;

    public:
        Component();
        virtual ~Component();
        virtual Component* construct();
        virtual Component* construct(Json::Value inValue);
        virtual Component* clone() {return new Component(*this);}

        bool enabled = true;
        std::string vanityName = "Unnamed Component";
        EntityID parentEntity = -1;

        //Auto generation of ID
        static ComponentID componentIDIncrementor;
        virtual ComponentID getID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
        static ComponentID getStaticID() {if(ID == 0) {ID = componentIDIncrementor++;} return ID;}
};

#endif // COMPONENT_H_INCLUDED
