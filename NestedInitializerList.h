
#ifndef NUNU_NESTEDINITIALIZERLISTS_H
#define NUNU_NESTEDINITIALIZERLISTS_H

#include <cstddef>
#include <initializer_list>


namespace //m3ta
{

    //- Traits.

    template<std::size_t T_levels, typename T=float>
    struct NestedInitializerListsTrait {
        using type = std::initializer_list<
            typename NestedInitializerListsTrait<T_levels - 1, T>::type
        >;
    };

    template<typename T>
    struct NestedInitializerListsTrait<0,T> {
        using type = T;
    };

}
//- Aliases.

template<std::size_t T_levels,typename T=float>
using NestedInitializerLists = typename NestedInitializerListsTrait<T_levels, T>::type;

// } // namespace m3ta.

#endif // NUNU_NESTEDINITIALIZERLISTS_H
