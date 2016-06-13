#include "tempplayerControlComponent.h"

ComponentID PlayerControlComponent::ID;

PlayerControlComponent::PlayerControlComponent(){vanityName = "Player Control Component";}
PlayerControlComponent::~PlayerControlComponent(){}
PlayerControlComponent* PlayerControlComponent::construct(Json::Value inValue)
{
    speed = inValue["speed"].asDouble();
    forwardKey = inValue["forwardKey"].asInt();
    backKey = inValue["backKey"].asInt();
    leftKey = inValue["leftKey"].asInt();
    rightKey = inValue["rightKey"].asInt();
    upKey = inValue["upKey"].asInt();
    downKey = inValue["downKey"].asInt();
    return this;
}
