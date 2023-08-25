
#ifndef NUNU_LIST_H_
#define NUNU_LIST_H_
#include "./Pair.h"

#include <cstdlib>
#include <iostream>


template <typename T>
class List{
    private:

    T* data = nullptr;
    int full_size = 0; // size of allocated space (2^n)
    int size = 0; // size of array in allocated space (size <= full_size)

    void _increment(){
        if(size==full_size){
            if(!full_size){
                ++full_size;
                _allocate_list(); //full_size==1
            }
            else{
                full_size<<=1;
                _resize_list();
            }
        }
        ++size;
    }

    void _decrement(){
        if(size==(full_size>>1)+1){
            full_size>>=1;
            _resize_list();
        }
        --size;
    }

    void _allocate_list(){
        data = (T*) malloc(sizeof(T)*full_size);
    }

    void _resize_list(){
        data = (T*) realloc(data, sizeof(T)*full_size);
    }

    public:

    List(){}

    List(unsigned int length):size(length){
        full_size=1;
        while (full_size<length){
            full_size<<=1;
        }
        _allocate_list();
    }

    List(const T& val, int length):List(length){
        for (int i = 0; i < size; i++)
        {
            *(data+i) = val;
        }
    }

    // template <typename C>
    List(const std::initializer_list<T>& lst):List(lst.size()){
        auto xx = lst.begin(); //why doesnt it work when I do std::initializer_list<int>::iterator ?
        for(int i=0; i<size; ++i){
            *(data+i) = *(xx+i);
        }
    }

    // Copy Constructor
    template <typename C>
    List(const List<C>& lst): List(lst.length()){
        for(int i=0; i<size; ++i){
            *(data+i) = lst[i];
        }
    }


    ~List(){
        free(data);

    }

    void clear(){
        size=0;
        free(data);
        data = nullptr;
        full_size = 0;
    }

    friend std::ostream& operator<<(std::ostream& strm, const List &lst ){
        strm << "[";
        for(int i =0; i<lst.size-1; ++i){
            strm << lst[i] <<", ";
        }
        if(lst.size>0){
            strm << lst[lst.size-1];
        }
        strm << "]";
        return strm;
    }

    operator std::string() const {
        return "List<"
            + std::string(typeid(T).name()) + ">("
            + std::to_string(size) + ")";
    }

    bool isempty(){
        return !size;
    }

    bool empty(){
        return !size;
    }


    void append(const T& val){
        _increment();
        *(data+size-1) = val;
    }

    void push_back(const T& val){
        append(val);
    }

    void push(const T& val){
        append(val);
    }

    void remove_elem(T val){
        int i=0;
        for (; i < size; i++){
            if(val==*(data+i)){
                remove(i);
                return;
            }
        }
    }

    void remove(int rem_idx){
        T* last_elem = end();
        for (T el = data+rem_idx+1; el < last_elem; el++)
        {
            *el = *(el+1);
        }
        pop();
    }

    List<T>& operator=(const List<T>& lst) {
        resize(lst.length());
        for(int i=0; i<size; ++i){
            *(data+i) = lst[i];
        }
        return *this;
    }

    T& operator[](const unsigned int& idx) const {
        return *(data + idx);
    }

    T* operator()(const unsigned int& idx) const {
        return data+idx;
    }

    bool operator==(List<T> lst) const{
        if(lst.length() != size){
            return false;
        }
        for(int i=0; i<size; ++i){
            if(lst[i] != *(data+i)){
                return false;
            }
        }
        return true;
    }
    
    void resize(int x){
        full_size = size = x;
        _resize_list();
    }

    T* begin() const {
        return data;
    }

    T* end() const {
        return data + size;
    }

    void pop(){
        _decrement();
    }

    T& top() const {
        return *(data+size-1);
    }

    int length() const {
        return size;
    }

    // should be static?
    void print() const {
        std::cout<<"[";
        
        for(int i=0; i<size-1; ++i){
            std::cout<<data[i]<<",";
        }
        if(size>0){
            std::cout<<data[size-1];
        }
        std::cout<<"]"<<std::endl;
    }
};

#endif