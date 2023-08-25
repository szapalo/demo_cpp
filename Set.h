#ifndef NUNU_SET_H_
#define NUNU_SET_H_


#include <iostream>
#include "./Container.h"
#include "./List.h"

template <typename T>
class Set {
    private:

    Container<T> container;

    public:

    class Iterator : public Container<T>::Iterator{
        public:
        Iterator() : Container<T>::Iterator(){}
        Iterator(const Container<T>& c) : Container<T>::Iterator(c){}
        const T& operator*() const {
            return Container<T>::Iterator::operator*().key;
        }

    };

    Set(){}

    Set(const std::initializer_list<T>& lst){
        int size=lst.size();
        auto xx = lst.begin(); //why doesnt it work when I do std::initializer_list<int>::iterator ?
        for(int i=0; i<size; ++i){
            container.insert(*(xx+i));
        }
    }

    Set(const List<T>& lst){
        for(auto& elem : lst){
            container.insert(elem);
        }
    }
    Set(const Set<T>& other_set){
        for(auto& elem : other_set){
            insert(elem);
        }
    }
    ~Set(){}

    Set<T> operator=(const Set set){
        Set<T> result;
        for(auto&& elem : set){
            result.insert(elem);
        }
        return result;
    }

    void clear(){
        container.clear();
    }

    void print() {
        std::cout<<"printing"<<std::endl;
        for(auto&& elem : *this){
            std::cout<<elem<<" ";
        }
        std::cout<<std::endl;
    }

    void insert(const T& v){
        container.insert(v);
    }
    
    // void add(const T& v){
    //     container.insert(v);
    // }

    // const Pair<T,> find(const T& v) const{
    //     return container.find(v);
    // }

    bool find(const T& v) const{
        return container.find(v);
    }

    bool remove(const T& v){
        return container.remove(v);
    }

    bool isdisjoint(const Set<T>& set) const {
        /*
        {this} {set}
        */
        for(auto& elem: set){
            if(container.find(elem)){ return false;}
        }
        return true;
    }

    bool issubset(const Set<T>& set) const {
        /*
        { {this} set }
        */
        for(auto& elem: *this){
            if(!set.find(elem)){ return false;}
        }
        return true;
    }


    bool issuperset(const Set<T>& set) const {
        /*
        { this {set} }
        */
        for(auto& elem: set){
            if(!container.find(elem)){ return false;}
        }
        return true;
    }

    bool operator==(const Set<T>& set) const {
        auto it = container.begin();
        auto itend = container.end();
        for(auto& elem: set){
            if((*it).key != elem){
                return false;
            }
            ++it;
        }
        if(it==itend){
            return true;
        }
        return false;
    }

    void join(Set<T>& set){
        for(auto& elem : set){
            container.insert(elem);
        }
    }

    void intersect(Set<T>& set){
        for(auto& elem : set){
            if(!container.find(elem)){
                container.remove(elem);
            }
        }
    }

    void complement(const Set<T>& set){
        for(auto& elem : set){
            container.remove(elem);
        }
    }

    void disjunctive(Set<T>& set){
        for(auto& elem : set){
            if(container.find(elem)){
                container.remove();
            }
            else{
                container.insert(elem);
            }
        }
    }

    int length() const {
        return container.size();
    }

    friend std::ostream& operator<<(std::ostream& strm, const Set<T> &set ){
        strm << "{";
        for(auto& elem : set){
            strm << elem <<", ";
        }
        strm << "}";
        return strm;
    }

    Iterator begin() const {
        return Iterator(container);
    }

    Iterator end() const {
        return Iterator();
    }

};


#endif