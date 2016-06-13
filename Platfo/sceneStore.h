#ifndef SCENESTORE_H_INCLUDED
#define SCENESTORE_H_INCLUDED

#include "store.h"

class SceneStore : public Store
{
public:
    SceneStore();//Default values set
    ~SceneStore(){};

    std::string sceneFile;

    void loadStore(Json::Value inValue); //Load the actual data on a new thread
};

#endif // SCENESTORE_H_INCLUDED
