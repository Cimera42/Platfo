#ifndef SCENESTORE_H_INCLUDED
#define SCENESTORE_H_INCLUDED

#include "store.h"

class SceneStore : public Store
{
public:
    SceneStore();//Default values set
    ~SceneStore(){};
    void loadStore(Json::Value inValue);

    std::string sceneFile;
};

#endif // SCENESTORE_H_INCLUDED
