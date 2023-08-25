
#ifndef NUNU_THREAD_H_
#define NUNU_THREAD_H_

#include "List.h"
#include <thread>
#include <future>
// #include "ctpl/ctpl_stl.h" //what is this ? check this out!!!!!

#include <iostream>

#include <chrono>


unsigned int nthreads = std::thread::hardware_concurrency();

#define NTHREADS nthreads


// balanced multithreading
namespace para{

    namespace { //anonymous namespace - analogous to private
        template<typename L, typename F>
        void _process(L* lst, const F &lambda){
            for(auto& elem : *lst){
                lambda(elem);
            }
        }

        struct exec_wrapper
        {
            template <class... Args>
            auto operator()(Args&&... args)
            {
                return _process(std::forward<Args>(args)...);
            }
        };

        exec_wrapper EXEC;
    }

    // template<typename A, typename B, typename F>
    // void for_each(A &object, B length, F &lambda){

    //     int range = length;
    //     int num_threads = (NTHREADS < range) ? NTHREADS : range;

    //     List<List<int>> split_args_list(num_threads);

    //     List<std::thread> threads(num_threads);

    //     for(int i=0; i<num_threads; ++i){
    //         new (split_args_list(i)) List<int>();
    //     }

    //     int idx = 0;
    //     for(auto&& elem : object){
    //         ++idx;
    //         split_args_list[idx % num_threads].append(elem);
    //     }

    //     for(int i=0; i<num_threads; ++i){
    //         new (threads(i)) std::thread(EXEC, split_args_list(i), lambda);
    //     }

    //     for(auto&& thread : threads){
    //         thread.join();
    //     }

    // }

    template<typename T, typename F>
    void for_each_pool(T start, T end, const F &lambda){
        
        int range = end - start;
        int num_threads = (NTHREADS < range) ? NTHREADS/2 : range;

        ctpl::thread_pool p(4);
        // std::future<void> x= p;
        int num_pools = 4;
        List<std::future<void>> results(num_pools);
        for(T i=start; i!=end; ++i){
                results[i%num_pools] = p.push([&lambda,i](int i){lambda(i);});
        }
        for(int i=0; i<num_pools; ++i){
            results[i].get();
        }
        // for(T i=start; i!=end; ++i ){
        // }
        // List<List<int>> split_args_list(num_threads);
        // List<std::future<void>> threads(num_threads);
        // stpl::thread_pool p(num_threads);

        // for(int i=0; i<num_threads; ++i){
        //     new (split_args_list(i)) List<int>();
        // }

        // for(int i=0; i<range; ++i){
        //     split_args_list[i % num_threads].append(i + start);
        // }
        
        // for(int i=0; i<num_threads; ++i){
        //     new (threads(i)) std::thread(EXEC, split_args_list(i), lambda);
        // }

        // for(auto&& thread : threads){
        //     thread.join();
        // }
    }

    template<typename T, typename F>
    void for_each_async(T start, T end, const F &lambda){
        
        int range = end - start;
        int num_threads = NTHREADS; //(NTHREADS < range) ? NTHREADS : range;

        List<List<int>> split_args_list(num_threads);
        List<std::future<void>> threads(num_threads);
        for(int i=0; i<num_threads; ++i){
            new (split_args_list(i)) List<int>();
        }

        for(int i=0; i<range; ++i){
            split_args_list[i % num_threads].append(i + start);
        }
        
        for(int i=0; i<num_threads; ++i){
            threads[i] = std::async(
                std::launch::async, EXEC, split_args_list(i), lambda
            );
        }

        for(auto&& thread : threads){
            thread.get();
        }
    }

    template<typename T, typename F>
    void for_each(T start, T end, const F &lambda){
        
        int range = end - start;
        int num_threads = NTHREADS; //(NTHREADS < range) ? NTHREADS : range;

        List<List<int>> split_args_list(num_threads);
        List<std::thread> threads(num_threads);
        for(int i=0; i<num_threads; ++i){
            new (split_args_list(i)) List<int>();
        }

        for(int i=0; i<range; ++i){
            split_args_list[i % num_threads].append(i + start);
        }
        
        for(int i=0; i<num_threads; ++i){
            new (threads(i)) std::thread(EXEC, split_args_list(i), lambda);
        }

        for(auto&& thread : threads){
            thread.join();
        }
    }

    template<typename T, typename F>
    void for_each2(T start, T end, const F &lambda){
        
        int range = end - start ;

        int num_threads = (NTHREADS < range) ? NTHREADS : range;

        int quotient = range / num_threads;
        int remainder = range % num_threads;
    
        List<List<int>> split_args_list(num_threads);
        List<std::thread> threads(num_threads);
        for(int i=0; i<remainder; ++i){
            new (split_args_list(i)) List<T>(quotient+1);
        }
        for(int i=remainder; i<num_threads; ++i){
            new (split_args_list(i)) List<T>(quotient);
        }

        for(int i=0; i<range; ++i){
            split_args_list[i % num_threads] = (i + start);
        }
        // exec_wrapper e;
        for(int i=0; i<num_threads; ++i){
            new (threads(i)) std::thread(EXEC, split_args_list(i), lambda);
        }

        for(auto&& thread : threads){
            thread.join();
        }
    }

    // template<typename it, typename F>
    // void for_each(it* start, int n, F &lambda){
    //     for(int i=0; i<n; ++i){
    //         lambda(start+i);
    //     }
    // }

    // template<typename it, typename F>
    // void for_each(it& start, int n, F &lambda){
    //     for(int i=0; i<n; ++i){
    //         lambda(start++);
    //     }
    // }

    // template<typename F, typename arg>
    // void thread(F &lambda, arg &elem){

    // }

    // template<typename F, typename arg>
    // void thread(F &lambda, arg* elem){

    // }


}

// multitheading 
// DO NOT USE THIS ONE IT IS SLOW!!!!
namespace para_full{

    template<typename T, typename F>
    void for_each(T start, T end, const F &lambda){
        // auto t1 = std::chrono::high_resolution_clock::now();

        // int range = end - start;

        // List<std::thread*> threads(range);

        // for(int i=0; i<range; ++i){
        //     *threads(i) = new std::thread(lambda, i + start);
        // }

        // -----OR-------
        List<std::thread*> threads;
        for(T i=start; i<end; ++i){
            threads.append( new std::thread(lambda, i));
        }

        // auto t2 = std::chrono::high_resolution_clock::now();
        // auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        for(auto&& thread : threads){
            (*thread).join();
        }
        // return duration;

        // for(int i=start; i<end; ++i){
        //     lambda(i);
        // }

    }

    template<typename it, typename F>
    void for_each(it* start, it* end, F &lambda){
        List<std::thread*> threads;

        for(it i=start; i!=end; ++i){
            threads.append( new std::thread(lambda, i) );
        }

        for(auto&& thread : threads){
            (*thread).join();
        }

        // for(it i = start; i!=end; ++i){
        //     lambda(i);
        // }
    }

    template<typename A, typename F>
    void for_each(A& start, F &lambda){

        for(auto& elem : start){
            lambda(elem);
        }
    }

    template<typename A, typename F>
    void for_each(A* start, F &lambda){
        for(auto& elem : *start){
            lambda(elem);
        }
    }


    template<typename F, typename arg>
    void thread(F &lambda, arg &elem){
        // List<thread> list
    }

    template<typename F, typename arg>
    void thread(F &lambda, arg* elem){

    }

}


#endif