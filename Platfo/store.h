#ifndef STORE_H_INCLUDED
#define STORE_H_INCLUDED

#include <string>
#include <json/json.h>

//Holds the base object class
class Store
{
public:
    Store();
    virtual ~Store();
    virtual void loadStore(Json::Value inValue);

    bool correctlyLoaded = false; //Signal to detect incorrect loading

private:
    int usageCount = 0; //Internal counter for object deletion
    std::string internalName; //Name for loading
};

#endif // STORE_H_INCLUDED
