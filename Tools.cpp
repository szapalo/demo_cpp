
#include <iostream>

#ifndef NUNU_TOOLS
#define NUNU_TOOLS 

namespace Tools{

   template <typename T>
   bool is_pointer(T const &t)
   {
      return false;
   }

   template <typename T>
   bool is_pointer(T *t)
   {
      return true;
   }


   template <typename T>
   class Type{
      public:
      bool is_type(T const &t){
         return true;
      }
      template <typename X>
      bool is_type(X const &x){
         return false;
      }
      bool is_pointer(T* const t){
         return true;
      }
      template <typename X>
      bool is_pointer(X const &x){
         return false;
      }
   };


   namespace {
      template <class T>
      constexpr std::string_view type_name(T& x){
         using namespace std;
         #ifdef __clang__
            string_view p = __PRETTY_FUNCTION__;
            return string_view(p.data() + 34, p.size() - 34 - 1);
         #elif defined(__GNUC__)
            string_view p = __PRETTY_FUNCTION__;
         #  if __cplusplus < 201402
            return string_view(p.data() + 36, p.size() - 36 - 1);
         #  else
            return string_view(p.data() + 49, p.find(';', 49) - 49);
         #  endif
         #elif defined(_MSC_VER)
            string_view p = __FUNCSIG__;
            return string_view(p.data() + 84, p.size() - 84 - 7);
         #endif
      }
   }

   template <class T>
   constexpr std::string_view type_str(T& x){
      return type_name<decltype(x)>();
   }



}


// template <namespace T>
// namespace Test{
   // bool is_instance(T const &t){
   //    return true;
   // }
   // template <typename X>
   // bool is_instance(X const &t){
   //    return false;
   // }
// }
// #include <type_traits>
// #include <typeinfo>
// #ifndef _MSC_VER
// #   include <cxxabi.h>
// #endif
// #include <memory>
// #include <string>
// #include <cstdlib>
// template <class T>
// std::string
// type_name()
// {
//     typedef typename std::remove_reference<T>::type TR;
//     std::unique_ptr<char, void(*)(void*)> own
//            (
// #ifndef _MSC_VER
//                 abi::__cxa_demangle(typeid(TR).name(), nullptr,
//                                            nullptr, nullptr),
// #else
//                 nullptr,
// #endif
//                 std::free
//            );
//     std::string r = own != nullptr ? own.get() : typeid(TR).name();
//     if (std::is_const<TR>::value)
//         r += " const";
//     if (std::is_volatile<TR>::value)
//         r += " volatile";
//     if (std::is_lvalue_reference<T>::value)
//         r += "&";
//     else if (std::is_rvalue_reference<T>::value)
//         r += "&&";
//     return r;
// }

// int main(){
//    int* x = nullptr;
//    int y=10;
//    std::cout<<Tools::is_pointer(x)<<std::endl;
//    std::cout<<Tools::is_pointer(y)<<std::endl;
   
//    Tools::Type<int> inspector_int;

//    std::cout<<inspector_int.is_type(x)<<std::endl;
//    std::cout<<inspector_int.is_type(y)<<std::endl;
//    std::cout<<inspector_int.is_pointer(x)<<std::endl;
//    std::cout<<inspector_int.is_pointer(y)<<std::endl;
//    // std::cout<<type_name<decltype(y)>()<std::endl;
   
// }
int& foo_lref();
int&& foo_rref();
int foo_value();

#include "./List.h"
int main()
{
   List<int> l;
   int i = 0;
   const int ci = 0;
   std::cout<<typeid(int).name()<<std::endl;
   std::cout << "decltype(int) is " << Tools::type_name<int>() << '\n';
   std::cout << "decltype(l) is " << Tools::type_name<decltype(l)>() << '\n';
   std::cout << "decltype(i) is " << Tools::type_name<decltype(i)>() << '\n';
   std::cout << "decltype((i)) is " << Tools::type_name<decltype((i))>() << '\n';
   std::cout << "decltype(ci) is " << Tools::type_name<decltype(ci)>() << '\n';
   std::cout << "decltype((ci)) is " << Tools::type_name<decltype((ci))>() << '\n';
   std::cout << "decltype(static_cast<int&>(i)) is " << Tools::type_name<decltype(static_cast<int&>(i))>() << '\n';
   std::cout << "decltype(static_cast<int&&>(i)) is " << Tools::type_name<decltype(static_cast<int&&>(i))>() << '\n';
   std::cout << "decltype(static_cast<int>(i)) is " << Tools::type_name<decltype(static_cast<int>(i))>() << '\n';
   std::cout << "decltype(foo_lref()) is " << Tools::type_name<decltype(foo_lref())>() << '\n';
   std::cout << "decltype(foo_rref()) is " << Tools::type_name<decltype(foo_rref())>() << '\n';
   std::cout << "decltype(foo_value()) is " << Tools::type_name<decltype(foo_value())>() << '\n';


   std::cout << "decltype(l) is " << Tools::type_str(l) << '\n';
   std::cout << "decltype(i) is " << Tools::type_str(i) << '\n';
   std::cout << "decltype((i)) is " << Tools::type_str((i)) << '\n';
   std::cout << "decltype(ci) is " << Tools::type_str(ci) << '\n';
   std::cout << "decltype((ci)) is " << Tools::type_str((ci)) << '\n';
   // std::cout << "decltype(foo_lref()) is " << Tools::type_str(foo_lref()) << '\n';
   // std::cout << "decltype(foo_rref()) is " << Tools::type_str(foo_rref()) << '\n';
   // std::cout << "decltype(foo_value()) is " << Tools::type_str(foo_value()) << '\n';


}

#endif

