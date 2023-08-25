

#include "./List.h"


template <typename T>
class IteratorBase : public List<T>{
    public:
};


class RandomIterator : public IteratorBase<T>{
    public:

};


class IteratorFactory {
    private:
    set<IteratorBase*> it_set;
    public:
    
    template<typename It>
    Iterator(const It& iterator){

    }

    ~Iterator(){
        for(auto& elem : it_set){
            ~elem;
        }
    }

    template<typename It>
    It begin(){

    }
};