


#include <iostream>
#include <vector>

class A{
    public:
    std::vector<int> v;
    A(std::initializer_list<int> x){
        std::cout<<x.size()<<std::endl;
        v.resize(x.size());
        std::initializer_list<int>::iterator it=x.begin();
        for(int i=0; i<x.size(); ++i){
            v[i] = (*it+i);
        }
    }
};

int main(){

    A a = {1,2,3};
    for(int i =0; i<a.v.size(); ++i){
        std::cout<<a.v[i]<<std::endl;
    }

}