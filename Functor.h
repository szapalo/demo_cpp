
#ifndef NUNU_FUNCTOR_H_
#define NUNU_FUNCTOR_H_

#include <iostream>
#include <functional>
#include <math.h>
namespace FunctorNamespace{

    class BaseFunctor{
        private:
        float delta_h = 0.00001;
        float delta_h2 = delta_h*2;

        public:

        BaseFunctor(){}

        float operator()(float x){return 0;}

        float derivative(float x){
            return (operator()(x+delta_h) - operator()(x-delta_h))/(delta_h2);
        }
    };
}

// class SubFunctor: public FunctorNamespace::BaseFunctor{
//     private:
//     BaseFunctor parent_functor;
//     public:

//     // SubFunctor(SubFunctor& f):parent_functor(f){}

//     float operator()(float x){
//         return parent_functor.derivative(x);
//     }
// };


// class LambdaFunctor : public FunctorNamespace::BaseFunctor{
//     public:
//     LambdaFunctor(std::function<float(float)> func_):func(std::move(func_)) {}

//     const std::function<float(float)> func;
//     float operator()(float x){ return func(x); }
// };



////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Common Functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// class Addition{};

// class Power: public FunctorNamespace::BaseFunctor{
//     Power(float exp){}
// };

// class Log : public baseFunctor{
    
// };


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Activation Functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Sigmoid : public FunctorNamespace::BaseFunctor{
    private:
    public:

    float operator()(float x){
        return 1/(1+std::exp(-x));
    }

    float derivative(float x){
        float sigma = operator()(x);
        return sigma*(1-sigma);
    }

};

class BinaryStep : public FunctorNamespace::BaseFunctor {
    private:

    public:
    float operator()(float x){
        return (x<0)? 0 : 1 ;
    }
};

class TanH : public FunctorNamespace::BaseFunctor {
    private:

    public:
    float operator()(float x){
        return 2/(1-std::exp(-2*x)) - 1;
    }
    
    float derivative(float x){
        float sigma = operator()(x);
        return 1 - sigma*sigma;
    }

};


class ReLU : public FunctorNamespace::BaseFunctor {
    private:

    public:
    float operator()(float x){
        return (x<0)? 0 : x;
    }
    
    float derivative(float x){
        return (x<0)? 0 : 1;
    }

};

class LeakyReLU : public FunctorNamespace::BaseFunctor {
    private:
    public:
    float operator()(float x){
        return (x<0)? 0.01*x : x;
    }
    
    float derivative(float x){
        return (x<0)? 0.01 : 1;
    }
};

class ParametricReLU : public FunctorNamespace::BaseFunctor {
    private:
    int alpha;
    public:
    ParametricReLU(float alpha):alpha(alpha){}
    float operator()(float x){
        return (x<0)? alpha*x : x;
    }
    
    float derivative(float x){
        return (x<0)? alpha : 1;
    }
};

class CappedReLU : public FunctorNamespace::BaseFunctor{
    private:
    int alpha;
    public:
    CappedReLU(float alpha=6):alpha(alpha){}
    float operator()(float x){
        return (x<0)? 0 : (x<alpha)? x : alpha;
    }
    
    float derivative(float x){
        return (x<0 || x>alpha)? 0 : 1;
    }

};

class ELU : public FunctorNamespace::BaseFunctor {
    private:
    float alpha;
    public:
    ELU(float alpha=1):alpha(alpha){}
    float operator()(float x){
        return (x<0)? alpha*(1-std::exp(x)) : x;
    }
    
    float derivative(float x){
        return (x<0)? operator()(x) + alpha : 1;
    }

};

class Swish : public FunctorNamespace::BaseFunctor {
    private:
    public:
    float operator()(float x){
        return x/(1+std::exp(-x));
    }

    float derivative(float x){
        float tmp = std::exp(x) + 1;
        return (x-1)/tmp - x/(tmp*tmp) + 1;
    }

};



// int main(){
//     LambdaFunctor a([](float x ){ return x+1; });
//     std::cout<<a.func(44)<<std::endl;

//     SubFunctor subfun(a);

//     std::cout<<a.derivative(1)<<std::endl;
// }


#endif