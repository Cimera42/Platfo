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

    std::string storeName; //Name for loading
    bool correctlyLoaded = false; //Signal to detect incorrect loading
    int usageCount = 0; //Internal counter for object deletion
};

#endif // STORE_H_INCLUDED
