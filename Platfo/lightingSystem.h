#ifndef LIGHTINGSYSTEM_H_INCLUDED
#define LIGHTINGSYSTEM_H_INCLUDED

#include "system.h"
#include <glm/glm.hpp>

struct DirectionalLightGroup
{
    int count = 0;
    std::vector<glm::vec3> direction;
    std::vector<float> intensity;
    std::vector<glm::vec3> colour;
};

struct PointLightGroup
{
    int count = 0;
    std::vector<glm::vec3> location;
    std::vector<float> intensity;
    std::vector<float> attenuation;
    std::vector<glm::vec3> colour;
};

struct SpotLightGroup
{
    int count = 0;
    std::vector<glm::vec3> location;
    std::vector<glm::vec3> direction;
    std::vector<float> intensity;
    std::vector<float> attenuation;
    std::vector<glm::vec2> angle;
    std::vector<glm::vec3> colour;
};

class LightingSystem : public System
{
    private:
        static SystemID ID;

    public:
        LightingSystem();
        virtual ~LightingSystem();

        DirectionalLightGroup compileDirectional();
        PointLightGroup compilePoint();
        SpotLightGroup compileSpot();

        //Auto generation of ID
        SystemID getID() {if(ID == 0) {ID = systemIDIncrementor++;} return ID;}
        static SystemID getStaticID() {if(ID == 0) {ID = systemIDIncrementor++;} return ID;}
};

#endif // LIGHTINGSYSTEM_H_INCLUDED
