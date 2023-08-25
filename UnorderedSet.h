#ifndef NUNU_UNORDEREDSET_H_
#define NUNU_UNORDEREDSET_H_

#define TABLE_SIZE 16
#define LOAD_FACTOR 1.0

#include <iostream>
#include "./List.h"
#include "./LinkedList.h"

namespace {
    template <typename K, typename T=K>
    class BaseEquator{
        public:
        static const bool equal(const K& k, const T& t ){
            return k==t;
        }
        static const K& get_key(const T& t){
            return t;
        }
    };
}

template <
    typename K, typename T=K, typename Comparator=BaseEquator<K,T>,
    typename LL = BaseLinkedList<K,T,Comparator>
>
class BaseUnorderedSet {
    private:

    static const int& _hashing(const K& k) { 
        //should be overloaded for specific cases. // ALSO SHOULD BE STATIC
        return k;
    }

    size_t _num_blocks = 16; //size of 
    float _max_load_factor = 1.0; // max_load_factor
    float _capacity;
    int _size = 0;

    List<LL> _blocks;


    void _expand(){
        int prev_size = _num_blocks;
        _num_blocks<<=1;
        _capacity = _num_blocks*_max_load_factor;
        _blocks.resize(_num_blocks);
        _restore(prev_size);
    }

    void _contract(){
        int prev_size = _num_blocks;
        _num_blocks>>=1;
        _capacity = _num_blocks*_max_load_factor;
        _restore(prev_size);
        _blocks.resize(_num_blocks);
    }

    void _restore(int prev_size){
        typename LL::Node* curr_node;
        typename LL::Node* next_node;
        size_t hash_val;
        for(int i=0; i<prev_size; ++i){
            curr_node = _blocks[i].head;

            while(curr_node){
                hash_val = _hashing(
                    Comparator::get_key(curr_node->data)
                ) % _num_blocks;
                
                if(i != hash_val){
                    next_node = _blocks[i].remove_node(curr_node, false);
                    _blocks[hash_val].push_back_node(curr_node);
                    curr_node = next_node;
                }else{
                    curr_node = curr_node->next;
                }
            }
        }
    }

    public:

    // public:
    class Iterator{
        private:
        LL* list_it;
        LL* end_it;        
        typename LL::Iterator linked_list_it;
        
        public:
        
        Iterator(
            LL* blocks_ptr,
            LL* blocks_end_ptr = nullptr,
            typename LL::Node* node_ptr=nullptr
        ):list_it(blocks_ptr), linked_list_it(node_ptr){
            end_it = blocks_end_ptr ? blocks_end_ptr : blocks_ptr; //special condition where blocks_ptr == end;
        }
        
        
        void operator++(){
            ++linked_list_it;
            if(!(linked_list_it.curr)){
                do{
                    ++list_it;
                }while(list_it != end_it && !(list_it->head));
                linked_list_it = (list_it != end_it)
                    ? typename LL::Iterator(list_it->head)
                    : typename LL::Iterator(nullptr);
            }
        }

        bool operator!=(const Iterator& it) const {
            return (
                it.list_it != list_it || it.linked_list_it != linked_list_it
            );
        }

        bool operator==(const Iterator& it) const {
            return (
                it.list_it == list_it && it.linked_list_it == linked_list_it
            ); 
        }

        const T& operator*(){
            return (*linked_list_it);
        }

    };

    BaseUnorderedSet(
        size_t num_blocks=TABLE_SIZE, float load_factor=LOAD_FACTOR
    ):
        _blocks(int(num_blocks)),
        _num_blocks(num_blocks),
        _max_load_factor(load_factor)
    {
        _capacity = _num_blocks*_max_load_factor;
    }
    ~BaseUnorderedSet(){}

    typename LL::Node* find(const K& k) const{
        int idx = _hashing(k) % _num_blocks;
        return _blocks[idx].find(k);
    }
    

    typename LL::Node* insert(const T& v){
        int key = Comparator::get_key(v);
        int idx = _hashing(key) % _num_blocks;
        LL* linked_list = &(_blocks[idx]);
        typename LL::Node* curr_node = linked_list->find(key);
        if(!curr_node){
            linked_list->push_back(v);
            curr_node = linked_list->tail;
            ++_size;
            if(_size > _capacity){
                _expand();
            }
        }
        return curr_node;
    }

    void remove(const K& k){
        int idx = _hashing(k) % _num_blocks;
        typename LL::Node* curr_node = _blocks[idx].find(k);
        if(curr_node){
            _blocks[idx].remove_node(curr_node);
            --_size;
            if( _size > TABLE_SIZE && _size<<1 < _capacity){
                _contract();
            }
        }
    }

    int length() const {
        return _size;
    }

    Iterator begin() const {
        LL* it = _blocks.begin();
        LL* end = _blocks.end();
        while(it != end && !(it->head)){
            ++it;
        }
        return (it != end) ? Iterator(it, end, it->head) : Iterator(end);
    }

    Iterator end() const {
        return Iterator(_blocks.end());
    }

    void clear(){
        for(auto&& block : _blocks){
            block.clear();
        }
        _blocks.resize(TABLE_SIZE);
    }

    void print() const {
        std::cout<<"(";
        for(auto&& e: (*this)){
            std::cout<<e<<", ";
        }
        std::cout<<")"<<std::endl;
    }
};


template <typename T>
using UnorderedSet = BaseUnorderedSet<T,T>;

#endif
