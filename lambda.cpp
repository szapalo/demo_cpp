

#include <iostream>
#include <functional>

auto g(int& i, int& j){
    std::cout<<i<<" -- "<<j<<std::endl;
    return i+j;
}

template<class... Args>
auto lam1(Args& ... args) {
    // auto x = [&args...]() { return g(args...); };
    // auto x = [=]() { return g(args...); };
    return [&]() { return g(args ... ); };
   // return x();
}


int a2(int& i, int& j){
    std::cout<<"i = "<<i <<" :: "<<&i<<std::endl;
    return i+j;
}

auto lam2(int& i, int& j) {
    return [&]() { return a2(i,j); };
}


// class Method{
//     public:
//     std::function<void()> function;
//     Method(std::function<void()> f) : function(f) {
//     void run(){}
// };

class C{
    public:
    int x;
    C(int x):x(x){
        std::cout<<"Start::"<<x<<std::endl;
    }

    void fun(){
        std::cout<<"C.fun :: "<<x<<std::endl;
    };
};

class Method { 
public:
    std::function<void()> function;
    Method(const std::function<void()>& f) : function(f) {
        // Handled in initializer list
    }
    void run(){
        function();
    }
};
#include "./List.h"

int main() {
    List<C*> cs;
    C* tmp_c = nullptr;
    List<Method*> methods;
    for(int i=2; i<11; ++i){
        tmp_c = new C(i);
        methods.push_back(new Method([=](){tmp_c->fun();}));
        cs.push_back(tmp_c);
    }
    for(auto& elem : methods){
        elem->run();
    }
    // C c(123);
    // Method m([=](){ std::cout<<"abc"<<std::endl;});
    // Method m([&](){ c.fun();});
    // m.run();
    for(auto& elem : cs){
        delete elem;
    }
}

// int main(){
//     int i=1, j=2;
//     auto x1 = lam1(i,j);
//     std::cout<<x1()<<std::endl;
//     auto x2 = lam2(i,j);
//     std::cout<<i<<j<<std::endl;
//     // auto x3 = [&]
//     std::cout<<x2()<<std::endl;



//     int x = 1;
//     auto valueLambda = [=]() { std::cout << x << std::endl; };
//     auto refLambda = [&]() { std::cout << x << std::endl; };
//     x = 13;
//     valueLambda();
//     refLambda();

//     Method m([](){std::cout<<"abc"<<std::endl;});

//     // Method m(1);
//     // int i=1;
//     // int j=2;
//     // std::cout<<a(i,j)<<std::endl;
// }



