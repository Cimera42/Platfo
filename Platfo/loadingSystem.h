#ifndef LOADINGSYSTEM_H_INCLUDED
#define LOADINGSYSTEM_H_INCLUDED

#include "system.h"
#include "store.h"
#include <pthread.h>
#include <map>

class LoadingSystem : public System
{
    private:
        static SystemID ID;
        static void* callLoaderUpdate(void *arg){return ((LoadingSystem*)arg)->loaderUpdate();}
        void* loaderUpdate();

        std::map<std::string, Store*> loadedMap;
        //Loading
        std::vector<Store*> regStore;
        std::vector<void*> regLoad;
        std::vector<Json::Value> regJSON;
        //Unloading
        std::vector<void*> regUnload;

        int hasLoaded;
        std::string currentLoadName;
        pthread_mutex_t loadingMutex;
        pthread_t loadingThread;

    public:
        LoadingSystem();
        virtual ~LoadingSystem();

        template <typename T>
        bool load(T** inStore, Json::Value inValue) {
            //Recieves a pointers to stores within components
            //Expects a valid JSON for the currentStore
            try {
                //Register the object/store to be loaded in loadingSystem
                T* defaultStore = new T(); //default
                T* loadingStore = new T(); //one we're loading on
                regJSON.push_back(inValue);
                regStore.push_back(defaultStore);
                regStore.push_back(loadingStore);
                regLoad.push_back(inStore);
                *inStore = defaultStore; //Set default value
            }
            catch(std::exception e) {
                Logger()<<"Store loading mismatch. "<<e.what()<<" Please check"
                "you have the correct amount of JSON arguments for each store."<<std::endl;
                return false;
            }
            return true;
        }

        template <typename T>
        bool unload(T** inStore) {
            //Register the store to be unloaded at next non-blocked update
            regUnload.push_back(inStore);
            *inStore = nullptr;
            return true;
        }

        //Auto generation of ID
        SystemID getID() {if(ID == 0) {ID = systemIDIncrementor++;} return ID;}
        static SystemID getStaticID() {if(ID == 0) {ID = systemIDIncrementor++;} return ID;}

        void update();
};

#endif // LOADINGSYSTEM_H_INCLUDED

/*Questions:
    - What happens if the component/entity is deleted as an object is loading?
    - Just a reminder that the handling of multiple objects is to be done in the components,
    not in the loading system or in the store.
*/
