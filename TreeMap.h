

#include <iostream>
#include "./Container.h"
#include "./Pair.h"
// #include "./Variant.h"

template <typename K, typename V>
class TreeMap{
    private:
    Container<K,Pair<V*,TreeMap<K,V>*>> multi_set;

    class Iterator : public Container<K,Pair<V*,TreeMap<K,V>*>>::Iterator{
        private:
        public:
    };

    public:

    TreeMap(){}

    ~TreeMap(){}

    Pair<V*,TreeMap<K,V>*> operator[](K& key){
        return multi_set.find(key);
    }

    V* value(K& key){
        return multi_set.find(key).key;
    }

    TreeMap<K,V>* submap(K& key){
        return multi_set.find(key).value;
    }

    Iterator begin() const{
        return Iterator(multi_set);
    }
    
    Iterator end() const{
        return Iterator();
    }

    bool is_subtree(const V& idx){
        return this[idx]->value;
    }

    Pair<K*,TreeMap<K,V>*>& operator[](const V& idx){
        return *(multi_set.find(idx));
    }

    Pair<K*,TreeMap<K,V>*>& operator()(const V& idx){
        
    }


};
// class A{
//     public:
//     A(){}

//     infd

// }

int main(){
    // std::cout<<fun(1)<<std::endl;
    // std::cout<<fun(1)<<std::endl;
}