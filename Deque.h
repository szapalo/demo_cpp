#ifndef NUNU_DEQUE_H_
#define NUNU_DEQUE_H_

#include "./List.h"
#include "./LinkedList.h"



template <typename T>
class Deque {
    /*
                 ---next--> 
     (front) HEAD  ......  TAIL (back)
                 <--prev--

    [ 0 1 ... <head_idx> ... <block_size-1>] ... [ 0 1 ... <tail_idx> ... ]
    */
    private:

    const int block_size = 128;
    // int block_size = 128; // larger is faster . i'll keep it here.

    class Iterator {
        public:
        int idx;
        int block_size;
        typename LinkedList<T*>::Iterator linked_it;
        
        typename LinkedList<T*>::Iterator set_it(
            LinkedList<T*>& linked_blocks, bool end
        ){
            return end ? 
                typename LinkedList<T*>::Iterator(
                    idx ? linked_blocks.tail : nullptr
                )
            : linked_blocks.begin();
        }

        Iterator(
            int idx, int size, LinkedList<T*>& linked_blocks, bool end=false
        ):
            idx(idx),
            block_size(size),
            linked_it(set_it(linked_blocks, end)){
            // if(end){ ++(*this); }
        }

        void operator++(){
            (++idx)%=block_size;
            if(!idx){
                ++linked_it;
            }
        }

        bool operator!=(const Iterator& it) const{
            return (idx != it.idx || linked_it != it.linked_it);
        }

        bool operator==(const Iterator& it) const{
            return (idx == it.idx && linked_it == it.linked_it);
        }

        const T& operator*(){
            return *(*linked_it + idx);
        }
        const T* operator->(){
            return (*linked_it + idx);
        }
    };


    LinkedList<T*> linked_blocks;

    int head_idx = 0;
    int tail_idx = block_size-1;
    int size = 0;

    void _push_back_block(){
        linked_blocks.push_back((T*) malloc(sizeof(T)*block_size));
    }

    void _push_front_block(){
        linked_blocks.push_front((T*) malloc(sizeof(T)*block_size));
    }

    void _pop_back_block(){
        delete linked_blocks.back();
        linked_blocks.pop_back();
    }

    void _pop_front_block(){
        delete linked_blocks.front();
        linked_blocks.pop_front();
    }

    void _clear_members(){
        head_idx = 0; //font
        tail_idx = block_size-1; //back
        linked_blocks.clear();
    }

    public:

    Deque(){}

    ~Deque(){
        clear();
    }

    void clear(){
        for(auto& elem : linked_blocks){
            delete elem;
        }
        _clear_members();
    }

    // [ 0 1 ... <head_idx> ... <block_size-1>] ... [ ... <tail_idx> ... ]

    void push_back(const T& data){
        (++tail_idx)%=block_size;
        if(!tail_idx){
            _push_back_block();
        }
        linked_blocks.back()[tail_idx] = data;
        ++size;
    }

    void push_front(const T& data){
        if(!head_idx){
            _push_front_block();
        }
        head_idx += (block_size -1);
        head_idx%=block_size;
        linked_blocks.front()[head_idx] = data;
        ++size;
    }

    void pop_back(){
        if(!tail_idx){
            _pop_back_block();
        }
        tail_idx+=(block_size-1);
        tail_idx%=block_size;
        --size;
        if(!size){
            _clear_members();
        }
    }

    void pop_front(){
        (++head_idx)%=block_size;
        if(!head_idx){
            _pop_front_block();
        }
        --size;
        if(!size){
            _clear_members();
        }
    }

    T& front() const {
        return linked_blocks.front()[head_idx];
    }

    T& back() const {
        return linked_blocks.back()[tail_idx];
    }

    bool empty() const {
        return !size;
    }
    
    Iterator begin() {
        return Iterator(head_idx, block_size, linked_blocks);
    }

    Iterator end() {
        return Iterator(
            (tail_idx + 1) % block_size, block_size, linked_blocks, true
        );
    }
};


#endif