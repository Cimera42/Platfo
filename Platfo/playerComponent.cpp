#include "playerComponent.h"
#include "logger.h"
#include "typeConversion.h"

ComponentID PlayerComponent::ID;

PlayerComponent::PlayerComponent() {vanityName = "Player Component";}
PlayerComponent::~PlayerComponent(){}
PlayerComponent* PlayerComponent::construct(int inTeamID)
{
    teamID = inTeamID;

    return this;
}
PlayerComponent* PlayerComponent::construct(std::vector<std::string> inArgs)
{
    if(inArgs.size() == 1)
    {
        int team = stringToInt(inArgs[0]);

        if(team != -9999)
        {
            this->construct(team);
        }
        else
        {
            Logger() << "Invalid input to Player Component creation" << std::endl;
        }
    }
    else
    {
        Logger() << "Invalid number of arguments to Player Component creation" << std::endl;
    }

    return this;
}
