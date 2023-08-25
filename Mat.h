#ifndef NUNU_MAT_H_
#define NUNU_MAT_H_


#include "Tensor.h"
#include "./List.h"
#include <iostream>
#include <math.h>


template <typename T>
const List<T> concat(const List<T>& a, const List<T>& b){
    int a_length = a.length();
    int b_length = b.length();
    int new_length = a_length + b_length;
    List<T> new_list(new_length);
    for(int i=0; i<a_length; ++i){
        new_list[i] = a[i];
    }
    for(int i=b_length; i< new_length; ++i){
        new_list[i] = b[i - b_length];
    }
    return new_list;
}


template <typename T>
const List<T> rot(const List<T>& list, int steps=1){
    if(steps<0){
        steps = list.length()+steps;
    }
    if(!steps){
        List<T> new_list = list;
        return new_list;
    }
    List<T> new_list(list.length());
    for(int i=0; i<steps; ++i){
        new_list[i] = list[list.length()-steps+i];
    }
    for(int i=steps; i<list.length(); ++i){
        new_list[i] = list[i-steps];
    }
    // list.print();
    std::cout<<"ROTATE"<<std::endl;
    new_list.print();
    return new_list;
}

template <typename T>
const List<T> permute(const List<T>& list, const List<int>& permutation){
    if (permutation.length() != list.length()){
        throw "permutation does not match";
    }
    List<T> new_list(list.legth());
    for(int i=0; i<permutation.length(); ++i){
        new_list[permutation[i]] = list[i];
    }
    return new_list;
}

// template <int N, int M, typename T>
// void _permute(const Tensor<N,T>& new_tensor, const Tensor<N,T>& tensor, Tensor<M,T> permutation){
//     if(M==1){
//         for(int i=0; i<permutation.size(); ++i){
//             new_tensor[i] = tensor[permutation[i]]; 
//         }
//     }
//     else{
//         for(int i=0; i<permutation.size(); ++i){
//             _permute(new_tensor[i], tensor[])
//         }

//     }
// }


template <int N, typename T>
const Tensor<N,T> permute(const Tensor<N,T>& tensor, const List<int> permutation){
    List<int> tensor_dims = tensor.dims();
    Tensor<N,T> new_tensor(tensor_dims);
    for(int i=0; i<permutation.length(); ++i){
        new_tensor[i] = tensor[permutation[i]];
    }
}

template <typename T>
const List<T> range(const int length){
    List<T> new_list(length);
    for(int i=0; i<length; ++i){
        new_list[i] =i;
    }
    return new_list;
}

template <typename T>
const List<T> range(const int start, const int end, const int step){
    List<T> new_list();
    int steps = (end-start)/step;
    for(int i=0; i<steps; ++i){
        new_list.append(start+ i*step);
    }
    return new_list;
}


template <int N, typename T>
void mult(const Tensor<N,T>& a, const Tensor<N,T>& b){
    List<int> dim_a = a.dims();
    List<int> dim_b = b.dims();
    if (dim_a[dim_a.length()-1] != dim_b[0]){
        throw "Dimensions dont match";
    }
    List<int> new_dims = (dim_a.length()>=dim_b.length())? dim_a : dim_b;
    for(int i=0; i<dim_a.length()-1; ++i){
        new_dims[i] = dim_a[i];
    }
    for(int i=1; i<dim_b.length(); ++i){
        new_dims[i+dim_a.length()-1] = dim_b[i];
    }
    std::cout<<"REDIR"<<std::endl;
    new_dims.print();
    // Tensor<N,T> new_tensor = ;
    // return new_tensor
}

template <int N, typename T>
Tensor<N,T> transpose(const Tensor<N,T>& tensor){
    List<int> dims = tensor.dims();
    List<int> new_dims = dims;
    int tmp_swap = dims[0];
    new_dims[0] = dims[1];
    new_dims[1] = tmp_swap;

    int n = new_dims[0];
    int m = new_dims[1];

    Tensor<N,T> new_tensor(new_dims);
    for(int i=0; i<n; ++i){
        for(int j=0; j<m; ++j){
            new_tensor[i][j] = tensor[j][i];
        }
    }
    return new_tensor;
}



template <int N, typename T>
T sum(const Tensor<N,T>& tensor){
    int result = 0;
    for(auto& sub_tensor : tensor){
        result += sum(sub_tensor);
    }
}

template <typename T>
T sum(const Tensor<1,T>& tensor){
    int result = 0;
    for(auto& element : tensor){
        result += element;
    }
    return result;
}

// template <typename T>
// Tensor<1,T> sum(const Tensor<2, T>& tensor, int dim=-1){
    
// }


template <int N, typename T>
Tensor<N-1,T> sum(const Tensor<N,T>& tensor, int dim=-1){
    List<int> dimensions = tensor.dims();
    dimensions.pop();
    Tensor<N-1,T> new_tensor(dimensions);
    return new_tensor;
}


template<typename T>
T sum(const List<T>& list){
    T result = 0;
    for(int i=1; list.length(); ++i){
        result += list[i];
    }
    return result;
}



Tensor<2> cholesky(Tensor<2>& tensor){
    Tensor<2> result(2,2);
    result = 0;
    Tensor<1> v(2);
    for(int j=0; j<2; ++j){
        for(int i=0; i<2; ++i){
            v[i] = tensor[i][j];
            for(int k=0; k<j; ++k){
                v[i] -= tensor[j][k]*tensor[k][i];
            }
            result[i][j] = v[i]/sqrt(v[j]);
        }
    }
    return result;
}


#endif