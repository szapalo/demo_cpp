#include <iostream>
#include <functional>

int fun(int a)
{
//    std::cout << "running fun :: " << a << std::endl;
   return a;
}

//-------------------------------
// BEGIN decorator implementation
//-------------------------------

template <class> struct Decorator;

template <class R, class... Args>
struct Decorator<R(Args ...)>
{
    Decorator(const std::function<R(Args ...)>& f) : f_(f) {}

    R operator()(Args ... args){
        // std::cout << "BEGIN decorating...\n"; 
        R result = f_(args...);
        // std::cout << "END decorating\n";
        return result;
    }
    std::function<R(Args ...)> f_;
};

template<class R, class... Args>
Decorator<R(Args...)> makeDecorator(R (*f)(Args ...))
{
   return Decorator<R(Args...)>(std::function<R(Args...)>(f));
}
//-------------------------------
// END decorator implementation
//-------------------------------

//-------------------------------
// Sample functions to decorate.
//-------------------------------

//test
auto decorated1 = makeDecorator(fun);

///////////////////////////////////////////////////////////////// OR //////////

template<class T>
auto decorator2(T&& func)
{
    // we create a closure below
    auto new_function = [&func = std::forward<T>(func)](auto&&... args){
        // std::cout << "BEGIN decorating...\n"; 
        auto result = func(std::forward<decltype(args)>(args)...);   
        // std::cout << "END decorating\n";
        return result;
    };
    return new_function;
}


// test
auto decorated2 = decorator2(fun);



///////////////////////////////////////////////////////////////// OR //////////
///// lambda ////////////

template<class T>
auto decorator3(T func)
{
    // we create a closure below
    auto new_function = [func](auto&&... args){
        // std::cout << "BEGIN decorating...\n"; 
        auto result = func(args...);
        // std::cout << "END decorating\n";
        return result;
    };
    return new_function;
}


auto decorated3 = decorator3(fun);


#include <chrono>

int main(){
    auto start = std::chrono::steady_clock::now();

    for(int i=0; i<1000000; ++i){
        // decorated1(1);
        // decorated2(1);
        decorated3(1);

    }

    auto end = std::chrono::steady_clock::now();
 
    std::cout << "Elapsed time in nanoseconds: "
        << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
        << " ns" << std::endl;
 
    std::cout << "Elapsed time in microseconds: "
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
        << " µs" << std::endl;
 
    std::cout << "Elapsed time in milliseconds: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " ms" << std::endl;
 
    std::cout << "Elapsed time in seconds: "
        << std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
        << " sec" << std::endl;

}
