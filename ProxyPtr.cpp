#ifndef NUNU_PROXYPTR_H_
#define NUNU_PROXYPTR_H_

#include <iostream>

template <typename ObjType>
class ProxyPtr{
    private:
    ObjType* __instance_ptr = nullptr;
    
    void __init__(){
        if(!__instance_ptr){
            __instance_ptr = new ObjType();
        }
    }
    public:
    ProxyPtr(){}
    ~ProxyPtr(){
        delete __instance_ptr;
    }
    ObjType& operator*() const {
        __init__();
        return *__instance_ptr;
    }

    ObjType* operator->() const {
        __init__();
        return __instance_ptr;
    }
    
    void refresh(){
        ~ProxyPtr();
        __instance_ptr = new ObjType();
    }

};

struct A{
    int x;
    A(){
        std::cout<<"INST A"<<std::endl;
        x=3;
    }
    ~A(){
        std::cout<<"Desturct A"<<std::endl;
    }
};

int main(){
    ProxyPtr<A> p;
    std::cout<<"run"<<std::endl;
    std::cout<<(*p).x<<std::endl;
    std::cout<<p->x<<std::endl;
    return 0;
}

#endif