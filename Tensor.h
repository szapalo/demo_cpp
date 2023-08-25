
#ifndef NUNU_TENSOR_H_
#define NUNU_TENSOR_H_


#include "./NestedInitializerList.h"
#include <cstdarg>
#include <iostream>
#include <time.h>
#include "./List.h"

template<int N, typename T = float>
class Tensor{

    private:

    unsigned int* _dims = nullptr;
    Tensor<N-1, T>* data = nullptr;
    friend Tensor<N+1, T>;

    void _malloc_data(){
        data = (Tensor<N-1, T>*) malloc(sizeof(Tensor<N-1, T>)*(*_dims));

        for(int i=0; i<*_dims; ++i){
            new (data + i) Tensor<N-1, T>(_dims+1);
        }
    }

    void _clear(const bool clear_dims){
        for(auto& elem: *this){
            elem._clear(false);
        }
        free(data);
        if(clear_dims){
            free(_dims);
        }
    }

    Tensor(unsigned int* dims ):_dims(dims){
        _malloc_data();
    }

    public:

    Tensor(unsigned int arg,...){
        _dims = (unsigned int*) malloc(sizeof(unsigned int)*N);
        *_dims = arg;
        va_list args;
        va_start(args, arg);
        for (int i = 1; i < N; ++i) {
            *(_dims+i) = va_arg(args, unsigned int);
        }
        va_end(args);
        _malloc_data();
    }


    Tensor(const List<unsigned int>& lst){
        _dims = (unsigned int*) malloc(sizeof(unsigned int)*N);
        for(int i=0; i<N; ++i){
            *(_dims+i) = lst[i];
        }
        int size = *_dims;
        data = (Tensor<N-1, T>*) malloc(sizeof(Tensor<N-1, T>)*size);
        for(int i=0; i<size; ++i){
            new (data + i) Tensor<N-1, T>(_dims+1);
        }
    }

    /* Deep Copy */
    Tensor(const Tensor<N, T>& tensor){
        int size = tensor.length();
        data = (Tensor<N-1, T>*) malloc(sizeof(Tensor<N-1, T>)*size);
        for(int i=0; i<size; ++i){
            new (data + i) Tensor<N-1, T>(*tensor(i));
        }
    }

    Tensor(const NestedInitializerLists<N,T>& lst, unsigned int* d=nullptr){
        int size = lst.size();
        _dims = d? d : (unsigned int*) malloc(sizeof(unsigned int)*N);
        *_dims = size;
        data = (Tensor<N-1, T>*) malloc(sizeof(Tensor<N-1, T>)*size);
        auto xx = lst.begin(); //why doesnt it work when I do std::initializer_list<int>::iterator ?

        for(int i=0; i<size; ++i){
            new (data + i) Tensor<N-1, T>(*(xx+i), _dims+1);
        }
    }

    /* Destructor */
    ~Tensor(){
        _clear(true);
    }

    List<int> dims(){
        List<int> result(N);
        for(int i=0; i<N; ++i){
            result[i] = *(_dims+i);
        }
        return result;
    }

    void print(){
        for(auto& elem : *this){
            elem.print();
        }
        std::cout<<std::endl;
    }

    /* Auto range for loop */
    Tensor<N-1, T>* begin(){
        return data;
    }

    Tensor<N-1, T>* end(){
        return data + *_dims;
    }

    /* Index operators */
    Tensor<N-1, T>& operator[](const unsigned int &idx) const {
        return *(data + idx);
    }

    Tensor<N-1, T>* operator()(const unsigned int &idx) const {
        return data + idx;
    }

    /* Assign */

    template<typename C>
    void operator=(const C& val){
        for(int i=0; i<length(); ++i){
            *(data + i) = val;
        }
    }

    void operator=(Tensor<N, T>& mat){
        for(int i=0; i<length(); ++i){
            *(data + i) = mat[i];
        }
    }

    int length(){
        return *_dims;
    }


    /* Addition */
    template<typename C>
    void operator+=(const C& val){
        for(auto& elem: *this){
            elem += val;
        }
    }

    void operator+=(const Tensor<N, T>& mat){
        for(int i=0; i<length(); ++i){
            *(data + i) += mat[i];
        }
    }

    template<typename C>
    const Tensor<N, T> operator+(const C& val) const{
        Tensor<N, T> new_tensor = *this;
        new_tensor += val;
        return new_tensor;
    }

    const Tensor<N, T> operator+(const Tensor<N, T>& tensor){
        Tensor<N, T> new_tensor = *this;
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

    void operator-=(const Tensor<N, T>& mat){
        for(int i=0; i<length(); ++i){
            *(data + i) -= mat[i];
        }
    }

    template<typename C>
    const Tensor<N, T> operator-(const C& val) const{
        Tensor<N, T> new_tensor = *this;
        new_tensor -= val;
        return new_tensor;
    }

    const Tensor<N, T> operator-(const Tensor<N, T>& tensor){
        Tensor<N, T> new_tensor = *this;
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

    void operator*=(const Tensor<N, T>& tensor){
        for(int i=0; i<length(); ++i){
            *(data + i) *= tensor[i];
        }
    }

    template<typename C>
    const Tensor<N, T>& operator*(const C& val){
        Tensor<N, T> new_tensor = *this;
        new_tensor *= val;
        return new_tensor;
    }

    const Tensor<N, T>& operator*(const Tensor<N, T>& tensor){
        Tensor<N, T> new_tensor = *this;
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

    void operator/=(const Tensor<N, T>& tensor){
        for(int i=0; i<length(); ++i){
            *(data + i) /= tensor[i];
        }
    }

    template<typename C>
    const Tensor<N, T> operator/(const C& val){
        Tensor<N, T> new_tensor = *this;
        new_tensor /= val;
        return new_tensor;
    }

    const Tensor<N, T> operator/(const Tensor<N, T>& tensor){
        Tensor<N, T> new_tensor = *this;
        new_tensor /= tensor;
        return new_tensor;
    }

};



template <typename T>
class Tensor<1, T>{
    private:
    friend Tensor<2, T>;

    unsigned int* _dims = nullptr;
    T* data = nullptr;

    void _clear(const bool clear_dims){
        free(data);
        if(clear_dims){
            free(_dims);
        }
    }

    Tensor(unsigned int* dims):_dims(dims){
        data = (T*) malloc(sizeof(T)*(*_dims));
    }

    public:
    int length(){
        return *_dims;
    }

    Tensor(unsigned int dim){
        _dims = (unsigned int*) malloc(sizeof(unsigned int));
        *_dims = dim;
        data = (T*) malloc(sizeof(T)*dim);
    }

    /* Deep Copy */
    Tensor( const Tensor<1, T>& tensor){
        int size = tensor.length();
        data = (T*) malloc(sizeof(T)*size);
        for(int i=0; i<size; ++i){
            *(data + i) = tensor[i];
        }
    }

    Tensor(const List<unsigned int>& lst){
        _dims = (unsigned int*) malloc(sizeof(unsigned int));
        *_dims = lst[0];
        data = (T*) malloc(sizeof(T)*(*_dims));
    }

    Tensor(const NestedInitializerLists<1,T>& lst, unsigned int* d=nullptr){
        int size = lst.size();
        _dims = d? d : (unsigned int*) malloc(sizeof(unsigned int));
        *_dims = size;
        data = (T*) malloc(sizeof(T)*size);
        auto xx = lst.begin(); //why doesnt it work when I do std::initializer_list<int>::iterator ?
        for(int i=0; i<size; ++i){
            *(data + i) = *(xx+i);
        }
    }

    ~Tensor(){
        _clear(true);
    }

    List<int> dims(){
            List<int> result(1);
            result[0] = *_dims;
        return result;
    }

    friend std::ostream& operator<<(std::ostream &strm, const Tensor<1, T> &a) {
        return strm << "A(" << a.length() << ")";
    }

    void print(){
        for(auto& elem : *this){
            std::cout<<elem<<" ";
        }
        std::cout<<std::endl;
    }

    /* Auto range for loop */

    T* begin(){
        return data;
    }

    T* end(){
        return data + *_dims;
    }

    /* Index operators */

    T& operator[](const unsigned int &idx) const {
        return *(data + idx);
    }

    T* operator()(const unsigned int &idx) const {
        return data + idx;
    }

    /* Assign */

    void operator=(const T& val){
        for(int i=0; i<length(); ++i){
            *(data + i) = val;
        }
    }

    void operator=(const Tensor<1, T>& lst){
        for(int i=0; i<length(); ++i){
            *(data + i) = lst[i];
        }
    }

    /* Addition */

    void operator+=(const T& val){
        for(auto& elem: *this){
            elem+=val;
        }
    }

    void operator+=(const Tensor<1, T>& lst){
        for(int i=0; i<length(); ++i){
            data[i]+=lst[i];
        }
    }

    Tensor<1, T> operator+(const T& val){
        Tensor<1, T> new_tensor = *this;
        new_tensor += val;
        return new_tensor;
    }

    Tensor<1, T> operator+(const Tensor<1, T>& tensor){
        Tensor<1, T>* new_tensor = new Tensor<1, T>(*this);
        (*new_tensor) += tensor;
        return *new_tensor;
    }

    /* Subtraction */

    void operator-=(const T& val){
        for(auto& elem: *this){
            elem -= val;
        }
    }

    void operator-=(const Tensor<1, T>& lst){
        for(int i=0; i<length(); ++i){
            data[i] -= lst[i];
        }
    }

    Tensor<1, T> operator-(const T& val){
        Tensor<1, T> new_tensor = *this;
        new_tensor -= val;
        return new_tensor;
    }

    Tensor<1, T> operator-(const Tensor<1, T>& tensor){
        Tensor<1, T>* new_tensor = new Tensor<1, T>(*this);
        (*new_tensor) -= tensor;
        return *new_tensor;
    }


    /* Multiplication */
    void operator*=(const T& val){
        for(auto& elem: *this){
            elem *= val;
        }
    }

    void operator*=(const Tensor<1, T> lst){
        for(int i=0; i<length(); ++i){
            data[i] *= lst[i];
        }
    }

    Tensor<1, T> operator*(const T& val){
        Tensor<1, T> new_tensor = *this;
        (*new_tensor) *= val;
        return *new_tensor;
    }

    Tensor<1, T> operator*(const Tensor<1, T>& tensor){
        Tensor<1, T> new_tensor = *this;
        (*new_tensor) *= tensor;
        return *new_tensor;
    }

    /* Division */

    void operator/=(const T& val){
        for(auto& elem: *this){
            elem /= val;
        }
    }

    void operator/=(const Tensor<1, T> lst){
        for(int i=0; i<length(); ++i){
            data[i] /= lst[i];
        }
    }

    Tensor<1, T> operator/(const T& val){
        Tensor<1, T> new_tensor = *this;
        (*new_tensor) /= val;
        return *new_tensor;
    }

    Tensor<1, T> operator/(const Tensor<1, T>& tensor){
        Tensor<1, T> new_tensor = *this;
        (*new_tensor) /= tensor;
        return *new_tensor;
    }

};


#endif
