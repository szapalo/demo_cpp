#ifndef NUNU_MAP_H_
#define NUNU_MAP_H_

#include <iostream>
#include "./Container.h"
#include "./Pair.h"
#include "./List.h"

template<typename K, typename V>
class Map {
    private:
    Container<K,V> set;

    public:
    class Iterator : public Container<K,V>::Iterator{
        public:
        Iterator() : Container<K,V>::Iterator(){}
        Iterator(const Container<K,V>& c) : Container<K,V>::Iterator(c){}
    };

    Map(){}
    Map(const List<Pair<K,V>>& pair_list){
        for(auto& elem : pair_list){
            set.insert(elem.key, elem.value);
        }
    }

    Map(
        const std::initializer_list<Pair<K,V>>& init_list
    ):Map(List<Pair<K,V>>(init_list)){}

    Map(const List<K>& init_list, V value){
        for(auto& elem : init_list){
            set.insert(elem, value);
        }
    }
    
    Map(
        const std::initializer_list<K>& init_list, V value
    ):Map(List<K>(init_list) ,value){}

    Map(const Map<K,V>& map){
        for(auto& elem : map){
            set.insert(elem.key, elem.value);
        }
    }

    ~Map(){
        clear();
    }

    void clear(){
        set.clear();
    }

    const Map<K,V>& operator=(const Map<K,V>& map){
        set.clear();
        for(auto& elem : map){
            set.insert(elem.key, elem.value);
        }
        return *this;
    }


    // setter
    V& operator[](const K& idx){
        auto node = set.find(idx);
        if(!node){
            node = set.insert(idx);
        }
        return node->value;
    }

    //getter
    V operator()(const K& idx) const {
        return set.find(idx)->value;
    }


    bool operator==(const Map<K,V>& map) const {
        auto it = set.begin();
        auto itend = set.end();
        for(auto& elem : map){
            if(it==itend || *it != elem){
                return false;
            }
            ++it;
        }
        if(it==itend){
            return true;
        }
        return false;
    }

    bool operator!=(const Map<K,V>& map) const{
        return !(map==*this);
    }

    Iterator begin() const {
        return Iterator(set);
    }

    Iterator end() const {
        return Iterator();
    }

    bool remove(const K& idx){
        return set.remove(idx);
    }

    const Pair<K,V>* find(const K& idx) const {
        return set.find(idx);
    }

    bool isin(const K& idx) const{
        return set.find(idx);
    }

    int length() const {
        return set.size();
    }

    List<K> keys() const {
        List<K> result;
        for(auto& elem : set){
            result.append(elem.key);
        }
        return result;
    }

    List<Pair<K,V>> items() const {  // is this really necessary??
        List<Pair<K,V>> result;
        for(auto elem: set){
            result.append({elem->key, elem->value});
        }
        return result;
    }

    void update(const Map<K,V>& other_map){
        for(auto& elem : other_map){
            operator[](elem.key) = elem.value;
        }
    }

    void print(){
        std::cout<<"{\n";
        for(auto& elem : set){
            std::cout<<"\t"<<elem.key<<" : "<<elem.value<<",\n";
        }
        std::cout<<"}\n";
    }

    friend std::ostream& operator<<(std::ostream& strm, const Map<K,V> &map ){
        strm<<"{\n";
        for(auto& elem : map){
            strm<<"\t"<<elem.key<<" : "<<elem.value<<",\n";
        }
        strm<<"}\n";
        return strm;
    }

};


#endif
