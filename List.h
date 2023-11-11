
#ifndef NUNU_LIST_H_
#define NUNU_LIST_H_


#include <iostream>
#include <cstring> //memcpy
#include <stdexcept>

template <typename T>
class List {
    private:

    T* data = nullptr;
    unsigned int full_size = 0; // size of allocated space (2^n)
    unsigned int size = 0; // size of array in allocated space (size <= full_size)

    template <typename C>
    void _assign(const C& lst){
        for(int i=0; i<size; ++i){
            *(data+i) = lst[i];
        }
    }

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
        if(!size){return;}
        if(size==(full_size>>1)+1){
            full_size>>=1;
        }
        if(full_size){
            _resize_list();
            --size;
        }
        else{
            clear();
        }
    }

    void _allocate_list(){
        data = (T*) ::operator new(sizeof(T)*full_size);
    }

    void _resize_list(){
        // should be called only if full_size>0; and old full_size != new full_size;
        T* new_data = (T*) ::operator new(sizeof(T)*full_size);

        if(size < full_size){
            std::copy(data, data+size, new_data);
            // std::memcpy(new_data, data, sizeof(T)*size);
        }
        else{
            std::copy(data, data+full_size, new_data);
            // std::memcpy(new_data, data, sizeof(T)*full_size);
        }
        ::operator delete(data);
        data = new_data;
    }

    bool _set_full_size(unsigned int length){
        unsigned int tmp_val =0;
        // Set full_size = 2^n s.t. n>=full_size
        if(length){
            tmp_val=1;
            while (tmp_val<length){
                tmp_val<<=1;
            }
        }

        if(tmp_val!=full_size){
            full_size = tmp_val;        
            return true;
        }
        else{
            return false;
        }
    }

    public:

    List(){}

    List(int length):size(length){
        if(length < 0){
            throw std::underflow_error("Negative input size.");
        }
        if(_set_full_size(length)){
            _allocate_list();
        }
    }

    List(int length, const T& val):List(length){
        for (int i = 0; i < size; i++)
        {
            *(data+i) = val;
        }
    }

    List(const std::initializer_list<T>& lst):List(lst.size()){  
        auto it = lst.begin(); //why doesnt it work when I do std::initializer_list<int>::iterator ?
        for(int i=0; i<size; ++i){
            *(data+i) = *(it+i);
        }
    }
    // Copy Constructor of List<T> 
    // Note: For some reason, the templated version does not recognize List<T>.
    //       So we must define one specifically.
    //       Perhaps it wont be an issue once we derive from an abstract class.
    List(const List<T>& lst) : List(lst.length()){
        _assign(lst);
    }
    // Copy Constructor from miscelaneous List
    template <typename C>
    List(const List<C>& lst) : List(lst.length()){
        _assign(lst);
    }

    ~List(){
        ::operator delete(data);
    }

    void clear(){
        size=0;
        ::operator delete(data);
        data = nullptr;
        full_size = 0;
    }

    friend std::ostream& operator<<(std::ostream& strm, const List &lst ){
        strm << "[";
        unsigned int n = lst.length();
        if(n){
            for(int i=0; i<n-1; ++i){            
                strm << lst[i] <<", ";
            }
            strm << lst[n-1];
        }
        strm << "]";
        return strm;
    }

    operator std::string() const {
        return "List<"
            + std::string(typeid(T).name()) + ">("
            + std::to_string(size) + ")";
    }

    bool empty() const {return !size;}

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

    template <typename C>
    void extend(const List<C>& lst){
        int prev_size = size;
        resize(size+lst.length());

        for(int i=0; i<lst.length(); ++i){
            *(data+prev_size+i) = lst[i];
        }
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
        if(rem_idx <= 0){
            throw std::underflow_error("Negative input size.");
        }
        T* last_elem = end();
        for (T el = data+rem_idx+1; el < last_elem; el++)
        {
            *el = *(el+1);
        }
        pop();
    }

    List<T>& operator=(const List<T>& lst) {
        resize(lst.length());
        _assign(lst);
        return *this;
    }
    
    T& operator[](const unsigned int& idx) const {
        return *(data + idx);
    }

    T* operator()(const unsigned int& idx) const {
        return data+idx;
    }

    bool operator==(const List<T>& lst) const{
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
    bool operator!=(const List<T>& lst){
        return !(*this == lst);
    }

    void resize(int length){
        if(length < 0){
            throw std::underflow_error("Negative input length.");
        }
        // TODO: resize where full_size = 2^n s.t. n>=0
        if(_set_full_size(length)){
            if(!size){
                _allocate_list();
            }
            else{
                _resize_list();
            }
        }
        size = length;

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

    unsigned int length() const {
        return size;
    }

    // should be static?
    void print() const {
        std::cout<<"[";
        if(size){
            for(int i=0; i<size-1; ++i){
                std::cout<<data[i]<<",";
            }
            std::cout<<data[size-1];
        }
        std::cout<<"]"<<std::endl;
    }
};

#endif