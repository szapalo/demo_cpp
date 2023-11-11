
#ifndef NUNU_TENSOR_H_
#define NUNU_TENSOR_H_


#include "./NestedInitializerList.h"
#include "./List.h"

#include <cstdarg>
#include <iostream>
#include <stdexcept>
	
template<int N, typename T = float> requires (N>0 && N < 9)
class Tensor {

    private:

    unsigned int* _dims = nullptr;
    Tensor<N-1, T>* _data = nullptr;
    friend Tensor<N+1, T>;

    static void assert_dims(const List<int>& dims){
        int n = dims.length();
        if(n != N){
            throw std::range_error("Length of dims List does not match N.");
        }
        for(int i=0; i<n; ++i){
            if(dims[i] <= 0){
                throw std::underflow_error(
                    "Non-positive element of dims : dims[i] <= 0");
            }
        }
    }

    void _malloc_dims(const List<int>& dims){
        Tensor<N,T>::assert_dims(dims);
        _dims = new unsigned int[N];
        for(int i=0; i<dims.length(); ++i){
            *(_dims+i) = dims[i];
        }
    }

    void _malloc_data(){
        _data = (Tensor<N-1, T>*) ::operator new(sizeof(Tensor<N-1, T>)*(*_dims));
        for(int i=0; i<*_dims; ++i){
            new (_data + i) Tensor<N-1, T>(_dims+1);
        }
    }

    void _clear(const bool clear_dims){
        for(auto& elem: *this){
            elem._clear(false);
        }
        if(clear_dims){
            delete[] _dims;
        }
        ::operator delete(_data);

    }

    void _assign(const Tensor<N>& tensor){
        for(int i=0; i<length(); ++i){
            (_data + i)->_assign(tensor[i]);
        }
    }

    Tensor(unsigned int* dims):_dims(dims){
        _malloc_data();
    }

    public:

    Tensor(int arg,...){
        List<int> dims_lst(N, arg);
        va_list args;
        va_start(args, arg);
        for (int i = 1; i < N ; ++i) {
            dims_lst[i] = va_arg(args, int);
        }
        va_end(args);

        _malloc_dims(dims_lst);
        _malloc_data();
    }


    Tensor(const List<int>& lst){
        _malloc_dims(lst);
        _malloc_data();
    }

    /* Deep Copy */
    Tensor(const Tensor<N,T>& tensor):Tensor(tensor.dims()){
        int size = tensor.length();
        for(int i=0; i<size; ++i){
            *(_data + i) = tensor[i];
        }
    }

    Tensor(const NestedInitializerLists<N,T>& lst, unsigned int* d=nullptr){
        int size = lst.size();
        if(size == 0){
            throw std::underflow_error("NestedList is empty.");
        }
        _dims = d? d : new unsigned int[N];
        *_dims = size;
        _data = (Tensor<N-1, T>*) ::operator new(sizeof(Tensor<N-1, T>)*size);
        auto iter = lst.begin(); //why doesnt it work when I do std::initializer_list<int>::iterator ?

        for(int i=0; i<size; ++i){
            new (_data + i) Tensor<N-1, T>(*(iter+i), _dims+1);
        }
    }

    /* Destructor */
    ~Tensor(){
        _clear(true);
    }

    unsigned int dim(unsigned int i) const {
        return *(_dims+i);
    } 

    List<int> dims() const {
        List<int> result(N);
        for(int i=0; i<N; ++i){
            result[i] = *(_dims+i);
        }
        return result;
    }

    void print(char start=' ', char end=' ') const {
        int n = length()-1;
        std::cout<<start<<"{ \n";
        for(int i=0; i<n; ++i){
            (_data+i)->print('\t', ',');
        }
        (_data+n)->print('\t', ' ');
        std::cout<<" }"<<end<<"\n";
    }

    /* Auto range for loop */
    Tensor<N-1, T>* begin() const {
        return _data;
    }

    Tensor<N-1, T>* end() const {
        return _data + *_dims;
    }

    /* Index operators */
    Tensor<N-1, T>& operator[](const unsigned int &idx) const {
        return *(_data + idx);
    }

    Tensor<N-1, T>* operator()(const unsigned int &idx) const {
        return _data + idx;
    }

    /* Assign */
    // TODO: assign operator must return Type&
    template<typename C>
    Tensor<N,T>& operator=(const C& val){
        for(int i=0; i<length(); ++i){
            *(_data + i) = val;
        }
        return *this;
    }


    Tensor<N,T>& operator=(const Tensor<N,T>& mat){
        _assign(mat);
        return *this;
    }

    unsigned int length() const {
        return *_dims;
    }

    void resize(int new_size){
        if(new_size <= 0){
            throw std::underflow_error("Non-positive resize input.");
        }
        int curr_size = length();
        
        if(curr_size == new_size){ return; }
        if(curr_size > new_size){
            // delete excess Tensor<N-1,T>[new_size :: dims-1]
            for(int i=new_size; i<curr_size; ++i){
                (_data+i)->_clear(false);
            }
        }
        
        Tensor<N-1,T>* _new_ptr = (Tensor<N-1,T>*) ::operator new(
                sizeof(Tensor<N-1, T>)*(new_size)
            );        
        
        _data = _new_ptr;
        *_dims = new_size;
        
        if(curr_size < new_size ){
            for(int i=curr_size; i<new_size; ++i){
                new (_data + i) Tensor<N-1, T>(_dims+1);
            }
        }
    }

    /* Addition */
    template<typename C>
    void operator+=(const C& val){
        for(auto& elem: *this){
            elem += val;
        }
    }

    void operator+=(const Tensor<N,T>& mat){
        for(int i=0; i<length(); ++i){
            *(_data + i) += mat[i];
        }
    }

    template<typename C>
    const Tensor<N,T> operator+(const C& val) const {
        Tensor<N,T> new_tensor = *this;
        new_tensor += val;
        return new_tensor;
    }

    const Tensor<N,T> operator+(const Tensor<N,T>& tensor){
        Tensor<N,T> new_tensor = *this;
        new_tensor += tensor;
        return new_tensor;
    }

    /* Subtraction */

    template<typename C>
    void operator-=(const C& val){
        for(auto& elem: *this){
            elem -= val;
        }
    }

    void operator-=(const Tensor<N,T>& mat){
        for(int i=0; i<length(); ++i){
            *(_data + i) -= mat[i];
        }
    }

    template<typename C>
    const Tensor<N,T> operator-(const C& val) const{
        Tensor<N,T> new_tensor = *this;
        new_tensor -= val;
        return new_tensor;
    }

    const Tensor<N,T> operator-(const Tensor<N,T>& tensor){
        Tensor<N,T> new_tensor = *this;
        new_tensor -= tensor;
        return new_tensor;
    }


    /* Multiplication */

    template<typename C>
    void operator*=(const C& val){
        for(auto& elem: *this){
            elem *= val;
        }
    }

    void operator*=(const Tensor<N,T>& tensor){
        for(int i=0; i<length(); ++i){
            *(_data + i) *= tensor[i];
        }
    }

    template<typename C>
    const Tensor<N,T>& operator*(const C& val){
        Tensor<N,T> new_tensor = *this;
        new_tensor *= val;
        return new_tensor;
    }

    const Tensor<N,T>& operator*(const Tensor<N,T>& tensor){
        Tensor<N,T> new_tensor = *this;
        new_tensor *= tensor;
        return new_tensor;
    }

    /* Division */
    template<typename C>
    void operator/=(const C& val){
        for(auto& elem: *this){
            elem /= val;
        }
    }

    void operator/=(const Tensor<N,T>& tensor){
        for(int i=0; i<length(); ++i){
            *(_data + i) /= tensor[i];
        }
    }

    template<typename C>
    const Tensor<N,T> operator/(const C& val){
        Tensor<N,T> new_tensor = *this;
        new_tensor /= val;
        return new_tensor;
    }

    const Tensor<N,T> operator/(const Tensor<N,T>& tensor){
        Tensor<N,T> new_tensor = *this;
        new_tensor /= tensor;
        return new_tensor;
    }

};



template <typename T>
class Tensor<1,T> {
    private:
    friend Tensor<2, T>;

    unsigned int* _dims = nullptr;
    T* _data = nullptr;

    void _clear(const bool clear_dims){
        if(clear_dims){
            delete[] _dims;
        }
        ::operator delete(_data);

    }

    void _malloc_dims(int dim){
        if( dim <= 0){
            throw std::underflow_error("Non-positive dim input value.");
        }
        _dims = new unsigned int[1];
        *(_dims) = dim;
    }

    void _malloc_data(){
        _data = (T*) ::operator new(sizeof(T)*(*_dims));
    }

    void _assign(const Tensor<1> tensor){
        for(int i=0; i<length(); ++i){
            *(_data + i) = tensor[i];
        }
    }

    Tensor(unsigned int* dims):_dims(dims){
        _malloc_data();
    }

    public:
    unsigned int length() const {
        return *_dims;
    }

    Tensor(int dim){
        _malloc_dims(dim);
        _malloc_data();
    }

    /* Deep Copy */
    Tensor( const Tensor<1,T>& tensor):Tensor(tensor.dims()){
        int size = tensor.length();
        for(int i=0; i<size; ++i){
            *(_data + i) = tensor[i];
        }
    }

    Tensor(const List<int>& lst):Tensor(lst[0]){}

    Tensor(const NestedInitializerLists<1,T>& lst, unsigned int* d=nullptr){
        int size = lst.size();
        if(size == 0){
            throw std::underflow_error("NestedList is empty.");
        }
        _dims = d? d : new unsigned int[1];
        *_dims = size;
        _malloc_data();
        auto iter = lst.begin(); //why doesnt it work when I do std::initializer_list<int>::iterator ?
        for(int i=0; i<size; ++i){
            *(_data + i) = *(iter+i);
        }
    }

    ~Tensor(){
        _clear(true);
    }

    unsigned int dim(unsigned int i){
        return *(_dims);
    } 

    List<int> dims() const {
            List<int> result(1);
            result[0] = *_dims;
        return result;
    }

    void resize(int new_size){ // TO FIX!!!
        if(new_size <= 0){
            throw std::underflow_error("Non-positive resize input.");
        }
        if(length() == new_size){ return; }
        _clear(false);        
        *_dims = new_size;
        _data = (T*) ::operator new(sizeof(T)*(new_size));

    }


    friend std::ostream& operator<<(std::ostream &strm, const Tensor<1,T> &a) {
        return strm << "A(" << a.length() << ")";
    }

    void print(char start=' ', char end=' ') const {
        std::cout<<start<<"{ ";
        int n=length()-1;
        for(int i=0; i<length()-1; ++i){
            std::cout<<*(_data+i)<<", ";
        }
        std::cout<<*(_data+n)<<" }"<<end<<std::endl;
    }

    /* Auto range for loop */

    T* begin() const {
        return _data;
    }

    T* end() const {
        return _data + *_dims;
    }

    /* Index operators */

    T& operator[](const unsigned int &idx) const {
        return *(_data + idx);
    }

    T* operator()(const unsigned int &idx) const {
        return _data + idx;
    }

    /* Assign */
    Tensor<1,T>& operator=(const T& val){
        for(int i=0; i<length(); ++i){
            *(_data + i) = val;
        }
        return *this;
    }

    Tensor<1,T>& operator=(const Tensor<1,T>& lst){
        for(int i=0; i<length(); ++i){
            *(_data + i) = lst[i];
        }
        return *this;
    }

    /* Addition */

    void operator+=(const T& val){
        for(auto& elem: *this){
            elem+=val;
        }
    }

    void operator+=(const Tensor<1,T>& lst){
        for(int i=0; i<length(); ++i){
            _data[i]+=lst[i];
        }
    }

    Tensor<1,T> operator+(const T& val){
        Tensor<1,T> new_tensor = *this;
        new_tensor += val;
        return new_tensor;
    }

    Tensor<1,T> operator+(const Tensor<1,T>& tensor){
        Tensor<1,T> new_tensor = *this;
        new_tensor += tensor;
        return new_tensor;
    }

    /* Subtraction */

    void operator-=(const T& val){
        for(auto& elem: *this){
            elem -= val;
        }
    }

    void operator-=(const Tensor<1,T>& lst){
        for(int i=0; i<length(); ++i){
            _data[i] -= lst[i];
        }
    }

    Tensor<1,T> operator-(const T& val){
        Tensor<1,T> new_tensor = *this;
        new_tensor -= val;
        return new_tensor;
    }

    Tensor<1,T> operator-(const Tensor<1,T>& tensor){
        Tensor<1,T>* new_tensor = new Tensor<1,T>(*this);
        (*new_tensor) -= tensor;
        return *new_tensor;
    }


    /* Multiplication */
    void operator*=(const T& val){
        for(auto& elem: *this){
            elem *= val;
        }
    }

    void operator*=(const Tensor<1,T> lst){
        for(int i=0; i<length(); ++i){
            _data[i] *= lst[i];
        }
    }

    Tensor<1,T> operator*(const T& val){
        Tensor<1,T> new_tensor = *this;
        new_tensor *= val;
        return new_tensor;
    }

    Tensor<1,T> operator*(const Tensor<1,T>& tensor){
        Tensor<1,T> new_tensor = *this;
        new_tensor *= tensor;
        return new_tensor;
    }

    /* Division */

    void operator/=(const T& val){
        for(auto& elem: *this){
            elem /= val;
        }
    }

    void operator/=(const Tensor<1,T> lst){
        for(int i=0; i<length(); ++i){
            _data[i] /= lst[i];
        }
    }

    Tensor<1,T> operator/(const T& val){
        Tensor<1,T> new_tensor = *this;
        new_tensor /= val;
        return *new_tensor;
    }

    Tensor<1,T> operator/(const Tensor<1,T>& tensor){
        Tensor<1,T> new_tensor = *this;
        new_tensor /= tensor;
        return *new_tensor;
    }

};


#endif
