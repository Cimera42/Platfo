#include "loadingSystem.h"

SystemID LoadingSystem::ID;

LoadingSystem::LoadingSystem(){
    loading_lock = PTHREAD_MUTEX_INITIALIZER;
    hasLoaded = true;
}

LoadingSystem::~LoadingSystem(){}

void LoadingSystem::update() {
    if(hasLoaded) {
        if(regStore.size() > 0) {
            //All this is to be on a new thread
            //Get next item and start load

            //In reality we're gonna wanna create a new store,
            //do loading on that, and then swap out with the old
            //one to prevent mutex locking issues
            hasLoaded = false;
            regStore[0]->loadStore(regJSON[0]);
            //Once done, delete this item, and add to map for others to use
            regStore.erase(regStore.begin());
            regJSON.erase(regJSON.begin());

            //Need to decide how to get name/what name to use - file loc,
            //a seperate name, etc? Should the name be accessed here or
            //inside the store? Questions to consider.
            //loadedMap.insert(std::pair<std::string,Store*>(regJSON[0]["name"],regStore[0]));

            hasLoaded = true;
        }
    }
}
