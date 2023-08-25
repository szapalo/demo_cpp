

#include <iostream>


#include <type_traits>

template <typename A, typename B> //UNION ONLY ALLOWS built-in datatypes
union UnionVariant{
    A a;
    B b;

    UnionVariant(A a):a(a){std::cout<<"u.A"<<std::endl;}
    UnionVariant(B b):b(b){std::cout<<"u.B"<<std::endl;}

    // void operator=(A x){a=x;}
    // void operator=(B x){b=x;}
};


template <typename A, typename B>
struct Variant{
    bool type;
    UnionVariant<A,B> u;
    Variant(A x):type(true),u(x){std::cout<<"v.A"<<std::endl;}
    Variant(B x):type(false),u(x){std::cout<<"v.B"<<std::endl;}
    decltype(type? u.a:u.b) get(){
        return type? u.a: u.b;
    } 
};

int main(){
    // UnionVariant<double,std::string*> v(1.1);
    Variant<double,std::string> v(1.123);
    // Variant<double,std::string> v(1.1);
    std::cout<<v.get()<<std::endl;

}