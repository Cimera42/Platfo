#include "playerComponent.h"

ComponentID PlayerComponent::ID;

PlayerComponent::PlayerComponent() {vanityName = "Player Component";}
PlayerComponent::~PlayerComponent(){}
PlayerComponent* PlayerComponent::construct(Json::Value inValue)
{
    teamID = inValue["teamID"].asInt();

    return this;
}
