

#include <iostream>

template<typename T>
bool all(const T& x){
    for(auto& elem : x){
        if(!elem){
            return false;
        }
    }
    return true;
}

template<typename T>
bool any(const T& x){
    for(auto& elem : x){
        if(elem){
            return true;
        }
    }
    return false;
}

template<typename T>
bool none(const T& x){
    return !any(x);
}


template<typename C, typename T<C>>
bool in(const C, const T<>& x){
    for(auto& elem : x){
        if(elem == c){
            return true;
        }
    }
    return false;
}


int main(){
    
}