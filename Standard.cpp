

#include "./List.h"

namespace Standard {
    
    List<int> range(int start, int end, int increment=1 ){
        List<int> result;
        for(int i=start; i<end; i+=increment){
            result.push_back(i);
        }
        return result;
    }

    template <typename T>
    List<Pair<int, T>> enumerate(const List<T>& lst){
        List<Pair<int,T>> result;
        for(int i=0; i<lst.length(); ++i){
            result.push_back({i, lst[i]});
        }
        return result;
    }
};