#ifndef NUNU_MAPTREE_H_
#define NUNU_MAPTREE_H_

#include <iostream>
#include "./Map.h"
#include "./Pair.h"

template <int N, typename K, typename V>
class TreeMap : public Map<K, Map<K,TreeMap<N-1,K,V>>>{
    // private:
    // public:
};

template <typename K, typename V>
class TreeMap<1,K,V> : public Map<K,V>{};



int main(){
    // TreeMap<1,int,int> tm1;
    // tm1[1];
    // std::cout<<tm1.keys()<<std::endl;
    TreeMap<2,int,int> tm1({
        {1,{{2,2}}}
        // {2,{1,1}}
    });
    // Map<int,int> tm1({
    //     {1,2},
    //     {2,2}
    // });
    
    // Map<int,int> map({
    //     {1,2},
    //     {2,2},
    // });

    // TreeMap<2,int,int> tm2({
    //     {
    //         1,{{1,1}}
    //     },
    // });

}
#endif
