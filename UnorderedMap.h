#ifndef NUNU_UNORDEREDMAP_H_
#define NUNU_UNORDEREDMAP_H_

#include <iostream>
#include "./UnorderedSet.h"
#include "./Pair.h"

namespace {

    template <typename K, typename V>
    class PairEquator {
        public:
        static bool equal(const Pair<K,V>& lhs, const K& rhs) {
            return ( lhs.key == rhs );
        }
        static const K& get_key(const Pair<K,V>& t) {
            return t.key;
        }
    };

    template <typename K, typename V>
    using LinkedListPair = BaseLinkedList<K, Pair<K,V>, PairEquator<K,V>>;

    template <typename K, typename V>
    using UnorderedSetPair = BaseUnorderedSet<
                K, Pair<K,V>, PairEquator<K,V>, LinkedListPair<K,V>
    >;

}



template <typename K, typename V>
class UnorderedMap : public UnorderedSetPair<K,V> {
    private:

    public:

    // class Iterator : public UnorderedSetPair<K,V>::Iterator {};

    using UnorderedSetPair<K,V>::clear;
    using UnorderedSetPair<K,V>::begin;
    using UnorderedSetPair<K,V>::end;
    using UnorderedSetPair<K,V>::insert;

    UnorderedMap(){}
    UnorderedMap(const List<Pair<K,V>>& pair_list){
        for(auto& elem : pair_list){
            insert({elem.key, elem.value});
        }
    }

    UnorderedMap(const std::initializer_list<Pair<K,V>>& init_list)
        :UnorderedMap(List<Pair<K,V>>(init_list)){}

    UnorderedMap(const List<K>& init_list, V value){
        for(auto& elem : init_list){
            insert({elem, value});
        }
    }
    
    UnorderedMap(const std::initializer_list<K>& init_list, V value)
        :UnorderedMap(List<K>(init_list) ,value){}

    UnorderedMap(const UnorderedMap<K,V>& map){
        for(auto& elem : map){
            insert({elem.key, elem.value});
        }
    }

    const UnorderedMap<K,V>& operator=(const UnorderedMap<K,V>& map){
        clear();
        for(auto&& elem : map){
            insert({elem.key, elem.value});
        }
        return *this;
    }

    // setter
    V& operator[](const K& idx){
        Pair<K,V>* pair_ptr = find(idx);
        if(!pair_ptr){
            pair_ptr = &(insert(Pair<K,V>(idx, V()))->data);
        }
        // Pair<K,V> m(idx, 0);
        // Pair<K,V>* p = new Pair<K,V>(idx,V());
        return pair_ptr->value;
    }

    //getter
    V operator()(const K& idx) const {
        return find(idx)->value;
    }

    bool operator==(const UnorderedMap<K,V>& map) const {
        auto it = begin();
        auto itend = end();
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

    bool operator!=(const UnorderedMap<K,V>& map) const {
        return !(map == *this);
    }

    Pair<K,V>* find(const K& idx) const {
        auto node = UnorderedSetPair<K,V>::find(idx);
        return (node)? &(node->data) : nullptr;
    }

    bool isin(const K& idx) const {
        return UnorderedSetPair<K,V>::find(idx);
    }

    List<K> keys() const {
        List<K> result;
        for(auto& elem : *this){
            result.append(elem.key);
        }
        return result;
    }

    List<Pair<K,V>> items() const {  // is this really necessary??
        List<Pair<K,V>> result;
        for(auto& elem: *this){
            result.append({elem->key, elem->value});
        }
        return result;
    }

    void update(const UnorderedMap<K,V>& other_map){
        for(auto& elem : other_map){
            operator[](elem.key) = elem.value;
        }
    }

    void print() const {
        std::cout<<"{\n";
        for(auto& elem : *this){
            std::cout<<"\t"<<elem.key<<" : "<<elem.value<<",\n";
        }
        std::cout<<"}\n";
    }

    friend std::ostream& operator<<(
        std::ostream& strm, const UnorderedMap<K,V> &map
    ) {
        strm<<"{\n";
        for(auto& elem : map){
            strm<<"\t"<<elem.key<<" : "<<elem.value<<",\n";
        }
        strm<<"}\n";
        return strm;
    }

};


#endif