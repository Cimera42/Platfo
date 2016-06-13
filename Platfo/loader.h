#ifndef LOADER_H_INCLUDED
#define LOADER_H_INCLUDED

/**
- All loading from the program should be evoked through the loader.
- Any internal stores, like framebuffer textures may or may not go through the loader,
  that is up to the system/component to deal with creation and deletion.
- The loadingSystem will then deal with registered objects, by requesting the appropriate store.
- The components have control over the JSON and handling multiple objects. The loader only deals
  with one object at a time.
**/

#include <pthread.h>
#include <map>
#include <string>
#include "fileReader.h"
#include "store.h"
#include "openGLFunctions.h"
#include "globals.h"

/*
class StoreLoader{
public:
    StoreLoader();
    ~StoreLoader();
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

    //bool unload();
private:
    std::vector<Store*> regStore;
    std::vector<Json::Value> regJSON;
};

extern StoreLoader LoadStore;*/

/*
extern std::map<std::string, Store*> internalMap;
extern std::map<std::string, pthread_t> currentlyLoadingMap;
extern pthread_mutex_t internalMapMutex;
extern pthread_mutex_t currentlyLoadingMapMutex;

///Distribution Classes
template <class T>
class Load
{
private:
    static void *threadedLoad(void*);
public:
    //scene
    static bool Object(T** returnLoc, bool attemptLoad, Json::Value);//objects
};

template <class T>
class LoadJoin //Class to allow for paramter transfer to thread
{
public:
    T** returnLoc; //Location to return object class/store to
    std::string s; //Internal name
    bool attemptLoad; //Defines whether loadStore is attempted or not
    T* defaultValues; //Default to use whilst loading/loadStore not attempted
};

template <class T>
void* Load<T>::threadedLoad(void* inptr)
{
    LoadJoin<T> inParam;
    inParam = *((LoadJoin<T>*) inptr);
    std::string s = inParam.s;
    bool attemptLoad = inParam.attemptLoad;
    T** returnLoc = inParam.returnLoc;

    //Mutex lock the map of stores being loaded to prevent data race
    pthread_mutex_lock(&currentlyLoadingMapMutex);
    //Search for store in map
    //If it has been loaded at least once, it will exist here
    //The wait for it to finish will however immediately terminate
    std::map<std::string, pthread_t>::iterator loadingit = currentlyLoadingMap.find(s);
    if(loadingit != currentlyLoadingMap.end())
    {
        //Wait for the store to be loaded
        pthread_join(loadingit->second, NULL);
        //Matching store will now be in internalMap
    }
    //Set store as being loaded
    currentlyLoadingMap[s] = pthread_self();
    //Unlock mutex, as it is no longer needed to protect currentlyLoadingMap
    pthread_mutex_unlock(&currentlyLoadingMapMutex);

    //Loading is controlled by the string
    pthread_mutex_lock(&internalMapMutex);
    std::map<std::string, Store*>::iterator it = internalMap.find(s); //see if the object already exists
    pthread_mutex_unlock(&internalMapMutex);
    if(it != internalMap.end())
    {
        //if it exists, double check name is correct,
        if(it->second->internalName == s)
        {
            //if the name matches - regardless of attemptLoad, override and use this (only issue is if name has been altered)
            it->second->usageCount += 1;
            Logger() << s <<" already loaded. Now used "<< it->second->usageCount << " times. \n"<< std::endl;//Debugging
            //use this and return
            *returnLoc = static_cast<T*>(it->second);
            return NULL; //true;
        }
        else
        {
            //Name mismatch: Give error
            Logger()<<"Load Error: Name mismatch '"<<s<<"'. - Something has been altered. Returning nullptr. \n \n"; //cannot guarentee safety of the pointer.
            //use default and return
            *returnLoc = nullptr;
            return NULL; //false;
        }
    }
    else
    {
        //if it doesnt exist, check attemptLoad status
        if(!attemptLoad)
        {
            //if we dont want to use the store specific loading, we add default to map and return the defaultValues
            Logger()<< s <<" is new, but not attempting load... \n";
            pthread_mutex_lock(&internalMapMutex);
            internalMap.insert(std::pair<std::string, Store*>(s, inParam.defaultValues));
            pthread_mutex_unlock(&internalMapMutex);

            inParam.defaultValues->internalName = s;
            *returnLoc = inParam.defaultValues;
            return NULL;
        }
        else
        {
            //if we want to attemptLoad, we create it and load it!
            T* loadedValues = new T();
            loadedValues->loadStore(s);
            Logger() << s <<" is new. Attempting load... \n";//Debugging
            if(loadedValues->correctlyLoaded)
            {
                //check loading process succeded
                pthread_mutex_lock(&internalMapMutex);
                internalMap.insert(std::pair<std::string, Store*>(s, loadedValues));
                pthread_mutex_unlock(&internalMapMutex);

                loadedValues->internalName = s;
                //overide and delete and return
                *returnLoc = loadedValues;
                delete inParam.defaultValues;
                return NULL; //true;
            }
            else
            {
                //if something goes wrong in loading, return indication of failure
                delete loadedValues;
                //set returnLoc to point to default or just nullptr it?
                //Loading incomplete: Give error
                Logger()<<"Load Error: Cannot load object '"<<s<<"'. \n \n"; //Loading a new object hasnt worked - report it
                //use default and delete the new threaded one etc etc and return
                //*returnLoc = nullptr;
                Logger() << pthread_self() << " done" << std::endl;
                return NULL; //false;
            }
        }
    }

    delete inptr;
    return NULL;
}

//IMPLEMENTATION - needs to be moved and the acceptable classes controlled in .cpp
template <class T>
bool Load<T>::Object(T** returnLoc, bool attemptLoad, Json::Value s)
{
    //Create a new instance of the store
    T * defaultValues = new T();
    if(!MULTITHREADED_LOADING)
    {
        //defaultValues->loadStore(s);
        defaultValues->internalName = s["name"].asString();
    }
    *returnLoc = defaultValues;

    //Create paramters to send to secondary thread
    LoadJoin<T>* param = new LoadJoin<T>();
    param->returnLoc = returnLoc;
    param->s = s["name"].asString();
    param->attemptLoad = attemptLoad;
    param->defaultValues = defaultValues;

    if(MULTITHREADED_LOADING)
    {
        //Create a thread which does loading, and in the meantime - return the created instance of the store.
        pthread_t thread;
        pthread_create(&thread, NULL, threadedLoad, (void*) param);
        //pthread_join(thread, NULL);
    }
    else
    {
        threadedLoad(param);
    }

    return true;
}

template <typename T>
class Unload
{
public:
    //scene
    static bool Object(T** deletePtr);//objects
};

//IMPLEMENTATION - needs to be moved and the acceptable classes controlled in .cpp
template <class T>
bool Unload<T>::Object(T** deletePtr)
{
    //Deleting is controlled by pointers
    std::map<std::string, Store*>::const_iterator it;
    std::string key = "";
    T* editablePtr;
    for (it = internalMap.begin(); it != internalMap.end(); ++it) //loop through to find the existing pointer
    {
        if (*deletePtr == static_cast<T*>(it->second))
        {
            key = it->first;
            editablePtr = static_cast<T*>(it->second);
            break;
        }
    }
    if(it != internalMap.end())
    {
        //if it exists edit values and return
        editablePtr->usageCount -=1;
        if(editablePtr->usageCount == 0 && key == editablePtr->internalName)
        {
            delete editablePtr;
            internalMap.erase(key);
            *deletePtr = nullptr;
            return true;
        }
        *deletePtr = nullptr;
        return true;
    }
    Logger()<<"Unload Error: Cannot locate object. Returning nullptr. \n \n";
    *deletePtr = nullptr;
    return false;
}*/

#endif // LOADER_H_INCLUDED
