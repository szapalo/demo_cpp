#ifndef NUNU_POOL_H_
#define NUNU_POOL_H_

#include "./List.h"
#include <thread>
#include <mutex>

#include "./Deque.h"

#include <chrono>
#include <thread>
#include <functional>

int NUM_CORES = std::thread::hardware_concurrency();

/**
Deque<SubProcess<>> : [ ... SubProcess<> .. [front] ]
            |                               /    |   \
            |                            |/_    \|/   _\|
            |        List<Process> [PoolProcess[0], ... PoolProcess[num_cores-1]]
           \|/_____
            /      \
            |       |
           \|/      |
Process[i].run()___/

**/


//AMENDMENT NEEDED ! 
// "bool killed;" should be thread-safe. TO AVOID UNDEFINED BEHAVIOR.
// perhaps use std::atomic<bool>
// although primitive bool seems to work fine, it is not recommended to use it.

class RLock{
    private:

    int wait_milliseconds = 1;
    std::recursive_mutex* rm = nullptr;
    std::chrono::milliseconds wait_interval;
    
    public:
    RLock():wait_interval(wait_milliseconds){
        rm = new std::recursive_mutex();
    }

    ~RLock(){
        rm->unlock();
        delete rm;
    }

    void acquire(){
        while(!rm->try_lock()){
            std::this_thread::sleep_for(wait_interval);
        }
    }

    void release(){
        rm->unlock();
    }

    void unlock(){
        rm->unlock();
    }

};

class Method { 
    public:
    std::function<void()> function;
    Method(const std::function<void()>& f) : function(f) {}
    ~Method(){}

    void run(){
        function();
    }
};

class PoolQueue{
    private:
    RLock rlock;
    public:
    Deque<Method*> queue;
    

    PoolQueue(){};

    bool empty(){
        return queue.empty();
    }

    void clear(){
        for(auto&& method: queue){
            delete method;
        }
        queue.clear();
    }

    Method* pop(){
        Method* result = nullptr;
        rlock.acquire();
        if (!queue.empty()){
            result = queue.back();
            queue.pop_back();
        }
        // std::cout<<"release"<<std::endl;
        rlock.release();
        // std::cout<<" after release"<<std::endl;

        return result;
    }

    void push(Method* method){
        rlock.acquire();
        queue.push_front(method);
        rlock.release();
    }

    void push(const std::function<void()>& f){
        rlock.acquire();
        queue.push_front(new Method(f));
        rlock.release();
    }
    
};


class PoolProcess{
    private:
    public:
    bool started = false;
    bool killed = false;
    bool finished = false;
    std::chrono::nanoseconds wait_interval; 
    
    PoolQueue* queue;
    std::thread* process_thread = nullptr;

    PoolProcess(PoolQueue* queue):wait_interval(1),queue(queue){
        process_thread = new std::thread(_run,this);
    }

    ~PoolProcess(){
        delete process_thread;
    }

    void start(){
        // std::cout<<"starting thread"<<std::endl;
        // std::cout<<"set started to true"<<std::endl;
        
        started = true;
        // process_thread->detach();
    }

    void kill(){
        killed = true;
    }

    bool kill_join(){
        kill();
        return join();
    }


    bool join(){
        if(process_thread && process_thread->joinable()){
            process_thread->join();
            // delete process_thread;
            return true;
        }
        return false;
    }

    static void _run(PoolProcess* pool_process){
        // std::cout<<
        //    "Running::"<<pool_process->killed
        //    <<"::"<<pool_process->queue->empty()<<"--"<<std::endl;
        while(!pool_process->started && !pool_process->killed){
            // std::cout<<(!pool_process->started)
            // << " -- " << (!pool_process->killed)<<std::endl;
            std::this_thread::sleep_for(pool_process->wait_interval);
        }
        // std::cout<<"BEGIN"<<std::endl;

        Method* method = nullptr;
        while(!(pool_process->killed)){
            method = pool_process->queue->pop();
            // std::cout<<"POP->"<<method<<"<-"<<std::endl;
            if(method){
                method->run(); 
                delete method;
            }
            else{ break; }
        }
        pool_process->finished = true;
    }

};

class Pool {
    private:
    public:
    PoolQueue queue;
    List<PoolProcess*> poolprocess_insts;

    // Pool(int num_threads = 4 ){
    Pool(int num_threads = NUM_CORES){
        for(int i=0; i<num_threads; ++i){
            poolprocess_insts.append(new PoolProcess(&queue));
        }
    }

    ~Pool(){
        for(auto& elem : poolprocess_insts){
            free(elem);
        }
    }

    void start(){
        for(auto& poolprocess : poolprocess_insts){
            poolprocess->start();
        }
    }

    void join(){
        bool finished = false;
        for(auto& poolprocess : poolprocess_insts){
            finished &=(poolprocess->join());
        }
        // finished ? break : std::this_thread::sleep_for(10); //milliseconds
    }

    void start_join(){
        start();

        join();
    }

    void kill(){
        for(auto& poolprocess : poolprocess_insts){
            poolprocess->kill();
        }
        queue.clear();
    }

    void kill_join(){ // kill further processing, and wait for current runs in pool to finish. 
        bool finished = false;
        for(auto& poolprocess : poolprocess_insts){
            finished &=(poolprocess->kill_join());
        }
        queue.clear();
    }

    void push(const std::function<void()>& f){
        queue.push(new Method(f));
    }

    // void push(Method* method){
    //     queue.push(method);
    // }



};




#endif