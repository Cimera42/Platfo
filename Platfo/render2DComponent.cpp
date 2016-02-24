#include "render2DComponent.h"
#include "logger.h"
#include "typeConversion.h"

ComponentID Render2DComponent::ID;

Render2DComponent::Render2DComponent(){vanityName = "Render 2D Component";}
Render2DComponent::~Render2DComponent(){}
Render2DComponent* Render2DComponent::construct(glm::vec2 inStartUV, glm::vec2 inSize)
{
    //start position of UV
    startUV = glm::vec2(inStartUV.x, inStartUV.y);

    //Width and height of UV
    UVsize = glm::vec2(inSize.x, inSize.y);

    return this;
}
Render2DComponent* Render2DComponent::construct(std::vector<std::string> inArgs)
{
    if(inArgs.size() == 4)
    {
        float uvX = stringToFloat(inArgs[0]);
        float uvY = stringToFloat(inArgs[1]);

        float sX = stringToFloat(inArgs[2]);
        float sY = stringToFloat(inArgs[3]);

        if(uvX != -9999 && uvY != -9999 &&
           sX != -9999 && sY != -9999)
        {
            glm::vec2 uv = glm::vec2(uvX,uvY);
            glm::vec2 siz = glm::vec2(sX,sY);

            this->construct(uv,siz);
        }
        else
        {
            Logger() << "Invalid input to Render 2D Component creation" << std::endl;
        }
    }
    else
    {
        Logger() << "Invalid number of arguments to Render 2D Component creation" << std::endl;
    }

    return this;
}
