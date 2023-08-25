#ifndef NUNU_HEAP_H_
#define NUNU_HEAP_H_


#include <iostream>
#include "./List.h"

namespace { 

    template<typename T>
    class greaterComparator{
        public:
        bool operator() (const T& lhs, const T& rhs) const {
            return ( lhs > rhs );
        }
    };

    template<typename T>
    class smallerComparator{
        public:
        bool operator() (const T& lhs, const T& rhs) const {
            return ( lhs < rhs );
        }
    };
}


template <typename T, typename Comparator >
class Heap{
    private:
    List<T> lst;
    const Comparator comparator;

    protected:
    
    public:
    Heap(const Comparator& comp = Comparator{}):comparator(comp){}
    
    Heap(
        const Heap<T, Comparator>& heap, Comparator comp = Comparator{}
    ):
        Heap(comp)
    {
        for(auto& elem : heap){
            lst.push_back(elem);
        }
    }

    ~Heap(){}

    void clear(){
        lst.clear();
    }

    /*
        left child = 2*N+1
        right child = 2*N+2
        parent = (N-1)/2
    */
    
    void push(const T val){
        lst.push_back(val);
        int idx = lst.length()-1;
        int parent_idx;
        T parent_val;

        while( idx ){
            parent_idx = (idx-1)/2;
            parent_val = lst[parent_idx];
            if( comparator(parent_val, val) ){
                lst[idx] = val;
                break;
            }
            lst[idx] = parent_val;
            idx = parent_idx;
        }
        lst[idx] =val; 
    }

    const T& top() const {
        return lst[0];
    }

    void pop(){
        int size = lst.length();
        if(!size){ return; }
        int idx = 0;
        int child_idx = 0;
        T val = *(lst.end()-1);
        T child_val;
        lst.pop();
        --size;
        while(size){
            child_idx = 2*idx+1;
            if(child_idx >= size){
                lst[idx] = val;
                break;
            }
            child_val = lst[child_idx];

            if(child_idx+1 < size){
                if( comparator( lst[child_idx+1], child_val) ){ // comparator(left, right)
                    child_val = lst[++child_idx];
                }
            }
            if( comparator(child_val, val) ){
                lst[idx] = child_val;
                idx = child_idx;
            }
            else{
                lst[idx] = val;
                break;
            }
        }
    }


    bool empty(){
        return !lst.length();
    }

    const T* begin() const {
        return lst.begin();
    }

    const T* end() const {
        return lst.end();
    }

};

template<typename T>
using MaxHeap = Heap<T, greaterComparator<T>>;

template<typename T>
using MinHeap = Heap<T, smallerComparator<T>>;



#endif