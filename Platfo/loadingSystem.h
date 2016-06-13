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

        std::map<std::string, Store*> loadedMap;
        std::vector<Store*> regStore;
        std::vector<Json::Value> regJSON;

        int hasLoaded;
        pthread_mutex_t loading_lock;

    public:
        LoadingSystem();
        virtual ~LoadingSystem();

        template <typename T>
        bool load(T** inStore, Json::Value inValue) {
            //Recieves a pointers to stores within components
            //Expects a valid JSON for the currentStore
            try {
                //Register the object/store to be loaded in loadingSystem
                T* newStore = new T();
                regJSON.push_back(inValue);
                regStore.push_back(newStore);
                *inStore = newStore; //Set default value
            }
            catch(std::exception e) {
                Logger()<<"Store loading mismatch. "<<e.what()<<" Please check"
                "you have the correct amount of JSON arguments for each store."<<std::endl;
                return false;
            }
            return true;
        }

        //bool unload(std::string key);

        //Auto generation of ID
        SystemID getID() {if(ID == 0) {ID = systemIDIncrementor++;} return ID;}
        static SystemID getStaticID() {if(ID == 0) {ID = systemIDIncrementor++;} return ID;}

        void update();
};

#endif // LOADINGSYSTEM_H_INCLUDED

/*Questions:
    -What happens if the component/entity is deleted as an object is loading?
*/
