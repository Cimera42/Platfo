#include "loadingSystem.h"

SystemID LoadingSystem::ID;

LoadingSystem::LoadingSystem()
{
    loadingMutex = PTHREAD_MUTEX_INITIALIZER;
    hasLoaded = true;
}

LoadingSystem::~LoadingSystem(){}

void LoadingSystem::update()
{
    if(hasLoaded)
    {
        if(regUnload.size() > 0)
        {
            for(int i = 0; i < regUnload.size(); i++)
            {
                //Unload objects while not loading to prevent blocking issues
                Store* toUnload = *((Store**)regUnload[0]);
                //Find iterator key
                bool found;
                std::map<std::string,Store*>::iterator it = loadedMap.begin();
                while(it != loadedMap.end())
                {
                    if(toUnload == it->second)
                    {
                        found = true;
                        it->second->usageCount--;
                        if(it->second->usageCount == 0) {
                            //No longer in use, delete it
                            delete it->second;
                            loadedMap.erase(it);
                        }
                        break;
                    }
                }
                if(!found)
                    Logger()<<"Cannot find "<<toUnload->storeName<<" to delete. ERROR!"<<std::endl;

                //Clean up
                regUnload.erase(regUnload.begin());
            }
        }
        if(regLoad.size() > 0)
        {
            //All this is to be on a new thread
            hasLoaded = false;
            //pthread_create(&loadingThread, NULL, callLoaderUpdate, this);
            loaderUpdate();
        }
    }
}

void* LoadingSystem::loaderUpdate()
{
    Store* defaultStore = regStore[0];
    Store* loadedStore = regStore[1];

    //Store needs to have name to be valid
    if(regJSON[0].isMember("name"))
    {
        currentLoadName = regJSON[0]["name"].asString();
        //Check for existing name
        std::string toCheck = currentLoadName;
        std::map<std::string,Store*>::iterator it;
        it = loadedMap.find(toCheck);
        if(it != loadedMap.end())
        {
            pthread_mutex_lock(&loadingMutex);
            *((Store**)regLoad[0]) = it->second;
            it->second->usageCount++;
            pthread_mutex_unlock(&loadingMutex);

            delete defaultStore;
            delete loadedStore;

            Logger()<<"Existing Store: "<<currentLoadName<<"."<<std::endl;
        } else
        {
            //If not found create a new store
            Logger()<<"New Store: "<<currentLoadName<<"."<<std::endl;

            loadedStore->loadStore(regJSON[0]); //Load on new

            //Check loading and swap out default
            if(loadedStore->correctlyLoaded)
            {
                loadedStore->usageCount++;
                loadedStore->storeName = toCheck;

                pthread_mutex_lock(&loadingMutex);
                *((Store**)regLoad[0]) = loadedStore; //Swap
                pthread_mutex_unlock(&loadingMutex);

                delete defaultStore;

                //Add to map for others to use
                loadedMap.insert(std::pair<std::string,Store*>(loadedStore->storeName,loadedStore));
            } else
            {
                delete loadedStore;
                Logger()<<"There was an issue loading "<<currentLoadName<<", using default values."<<std::endl;
            }
        }
    } else
    {
        delete loadedStore;
        Logger()<<"No name supplied for the current store, using default values."<<std::endl;
    }
    //Clean up after loading
    regJSON.erase(regJSON.begin());
    regStore.erase(regStore.begin());
    regStore.erase(regStore.begin());
    regLoad.erase(regLoad.begin());

    pthread_mutex_lock(&loadingMutex);
    hasLoaded = true;
    pthread_mutex_unlock(&loadingMutex);

    return NULL;
}
