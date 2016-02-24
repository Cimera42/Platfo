#include "tempplayerControlComponent.h"
#include "logger.h"
#include "typeConversion.h"

ComponentID PlayerControlComponent::ID;

PlayerControlComponent::PlayerControlComponent(){vanityName = "Player Control Component";}
PlayerControlComponent::~PlayerControlComponent(){}
PlayerControlComponent* PlayerControlComponent::construct(float inSpeed, int inForward, int inBack, int inLeft, int inRight, int inUp, int inDown)
{
    speed = inSpeed;
    forwardKey = inForward;
    backKey = inBack;
    leftKey = inLeft;
    rightKey = inRight;
    upKey = inUp;
    downKey = inDown;
}
PlayerControlComponent* PlayerControlComponent::construct(std::vector<std::string> inArgs)
{
    if(inArgs.size() == 5)
    {
        float spe = stringToFloat(inArgs[0]);

        int forward = stringToInt(inArgs[1]);
        int back = stringToInt(inArgs[2]);
        int left = stringToInt(inArgs[3]);
        int right = stringToInt(inArgs[4]);
        int up = stringToInt(inArgs[5]);
        int down = stringToInt(inArgs[6]);

        if(spe != -9999 &&
           forward != -9999 && back != -9999 && left != -9999 && right != -9999 && up != -9999 && down != -9999)
        {
            this->construct(spe,forward,back,left,right,up,down);
        }
        else
        {
            Logger() << "Invalid input to Control Component creation" << std::endl;
        }
    }
    else
    {
        Logger() << "Invalid number of arguments to Control Component creation" << std::endl;
    }

    return this;
}
