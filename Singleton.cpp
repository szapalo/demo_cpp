
#ifndef NUNU_SINGLETON_H_
#define NUNU_SINGLETON_H_


#include <iostream>
#include <mutex>
#include <thread>

class Singleton{
    private:
    static Singleton* _singleton;

    Singleton(int x):x(x){}

    public:
    int x;

    static Singleton* init(int x){
        if(_singleton == nullptr){
            _singleton = new Singleton(x);
        }
        return _singleton;
    }

    /**
     * Singletons should not be cloneable.
    */
    Singleton(Singleton &other) = delete;
    /**
     * Singletons should not be assignable.
    */
    void operator=(const Singleton &other) = delete;
};

Singleton* Singleton::_singleton = nullptr;


////////////////////////////////////////////////////////////////////////////////

// threadsafe
class GlobalSingleton{
    private:
    
    static std::mutex _mutex;
    
    GlobalSingleton(int x):x(x){}
    static GlobalSingleton* _singleton;
    
    public:
    int x;

    static GlobalSingleton* init(int x){
        std::lock_guard<std::mutex> lock(_mutex);
        if(_singleton == nullptr){
            _singleton = new GlobalSingleton(x);
        }
        return _singleton;
    }

    /**
     * GlobalSingletons should not be cloneable.
    */
    GlobalSingleton(GlobalSingleton &other) = delete;
    /**
     * GlobalSingletons should not be assignable.
    */
    void operator=(const GlobalSingleton &other) = delete;
};

GlobalSingleton* GlobalSingleton::_singleton = nullptr;
std::mutex GlobalSingleton::_mutex;


#endif