#ifndef NUNU_STAT_H_
#define NUNU_STAT_H_


#include "./Tensor.h"
#include "./Mat.h"
#include "./List.h"
// #include <cmath.h>
#include <iostream>

#include "./Random.h"
#include <math.h>


// Normal distribution constants
#define SQRT_PI sqrt(M_PI)
#define SQRT_2_PI sqrt(M_2_PI)
#define NORMAL_B1 -0.0004406
#define NORMAL_B2 0.0418198
#define NORMAL_B3 0.9

// Inverse Normal distribution constants
#define a0 2.50662823884
#define a1 -18.61500062529
#define a2 41.39119773534
#define a3 -25.44106049637

#define b0 -8.47351093090
#define b1 23.08336743743
#define b2 -21.06224101826
#define b3 3.13082909833

#define c0 0.3374754822726147
#define c1 0.9761690190917186
#define c2 0.1607979714918209
#define c3 0.0276438810333863
#define c4 0.0038405729373609
#define c5 0.0003951896511919
#define c6 0.0000321767881768
#define c7 0.0000002888167364
#define c8 0.0000003960315187



class BaseDistribution{
    public:

    public:

    virtual float mean() = 0;
    virtual float stdev() = 0;
    virtual float variance() = 0;

    virtual float prob(float x) = 0;
    virtual float cdf(float x) = 0;
    virtual float random() = 0;

};


float sigmoid(float x){
    return 1.0/(1+exp(x));
}

// Probability distributions
class Normal{

    public:
    float mu;
    float sigma;
    float random_span;

    Normal(int mu=0, int sigma=1):mu(mu),sigma(sigma),random_span(sigma*5){}

    float prob(float x){
        return (sigma*SQRT_2_PI);
    }

    float cdf(float x){
        // -8 < Z < 8
        return sigmoid(
            SQRT_2_PI*(NORMAL_B1*pow(x,5)+NORMAL_B2*pow(x,3)+NORMAL_B3*x)
        );
    }

    // input from 0 to 1;
    float inverse_normal(float prob){
        float y = prob-0.5;
        float r, result;
        if(y<0.42 && y>-0.42){
            r = y*y;
            result = y*(((a3*r + a2)*r + a1)*r +a0)/((((b3*r +b2)*r +b1)*r +b0)*r +1);
        }
        else{
            r = prob;
            if(y>0){ r = 1-prob;}
            r = log(-log(r));
            result = c0 + r*(c1+r*(c2+r*(c3+r*(c4+r*(c5+r*(c6+r*(c7+r*c8)))))));

        }
        return result; //(result-mu)/sigma;
    }

    float mean(){
        return mu;
    }

    float stdev(){
        return sigma;
    }

    float random(){
        return inverse_normal(random_unit());
    }

    float zscore(float x){
        return (x-mu)/sigma;
    }
};

// class Student : public Normal{
//     public: 
//     int t;
//     Student(float mu, float sigma, int t): Normal(mu,sigma),t(t){}

// };

class Uniform{
    public:
    float _mean;
    float _stdev;
    float _prob;
    float length;

    Uniform(float length){
        length=length;
        _mean = length/2;
        _stdev = length/sqrt(12);
        _prob = 1/length;
    }

    float prob(float x=0){return _prob;}

    float cdf(float x){
        return x*_prob;
    }

    float mean(){return _mean;}

    float stdev(){return _stdev;}
    
    float random(){
        return length*(random_unit());
    }

};

class UniformInt: Uniform{
    public:
    UniformInt(float length): Uniform((int)length){}
    float random(){
        return uniform(length);
    }
};

class Binomial{
    public:
    float p;
    float q;
    int n;
    float _mean;
    float _stdev;

    static int binomial_coef(int n, int k){
        long long r = 1;
        if(k<<1 > n){
            k = n-k;
        }
        for(int d=1; d<=k; ++d){
            r *= n--;
            r /= d;
        }
        return int(r);
    }

    Binomial(float p, int n):p(p),q(1-p),_mean(n*p),_stdev(n*p*q){}

    float prob(int x){
        return binomial_coef(n,x)*pow(p,x)*pow(q,n-x);
    }

    float mean(){return _mean;}
    float stdev(){return _stdev;}

    float cdf(int x){
        // def binomial_cdf(x,n,p):
        //     cdf = 0
        //     b = 0
        //     for k in range(x+1):
        //         if k > 0:
        //             b += + np.log(n-k+1) - np.log(k) 
        //         log_pmf_k = b + k * np.log(p) + (n-k) * np.log(1-p)
        //         cdf += np.exp(log_pmf_k)
        //     return cdf

        float result = exp(n) + q; // simplification of exp(log_pmf_k) = exp(b + k * np.log(p) + (n-k) * np.log(1-p)) when k=0
        // exp(b + k * np.log(p) + (n-k) * np.log(1-p))
        // when k = 0 , then b==0;
        // exp(0 + 0*np.log(p) + (n-0)*np.log(q) )
        // exp(n*log(q)) = exp(n)+q 
        float b = 0;
        float log_pmf_k = 0;
        for(int k=1; k<=x; k++){
            b += log(n-k+1) - log(k);
            log_pmf_k = b + k*log(p) + (n-k)*log(q);
            result += exp(log_pmf_k);
        }
        return result;
    }

    float random(){
        int sum=0;
        for(int i=0; i<n; ++i){
            if(random_unit() > p){
                ++sum;
            }
        }
        return sum;
    }
};

long factorial(int x){
    long r=1;
    for(int i=2; i<=x; ++i){
        r*=i;
    }
    return r;
}

class Poisson {
    public:
    int lambda;
    double exp_lamdba;
    Poisson(float lambda):lambda(lambda),exp_lamdba(exp(-lambda)){}

    float prob(int x){
        return pow(lambda, x)*exp_lamdba/factorial(x);
    }

    float mean(){return lambda;}
    float stdev(){return sqrt(lambda);}
    float cdf(int x){
        float sum = 0;
        for(int i=0; i<x; ++i){
            sum += prob(i);
        }
        return sum;
    }

};


template <typename T>
T average(const List<T>& list){
    return sum(list)/list.length();
}

template <typename T>
T expectance(const List<T>& list){
    return average(list);
}

template <typename T>
T expectance(const List<T>& a, const List<T>& b){
    if(a.length() != b.length()){
        throw ("expectance :: dimensions dont match");
    }
    List<T> tmp_list = a;
    for(int i=0; i< a.length(); ++i){
        tmp_list[i] += b[i];
    }
    return average(tmp_list);

}

template <typename T>
T variance(const List<T>& list){
    T ave = average(list);
    int n = list.length();
    List<T> tmp_list = list;
    for(auto& elem : tmp_list){
        elem+=ave;
        elem*=elem;
    }
    return sum(tmp_list)/n/n;
}

template <typename T>
T covariance(const List<T>& a, const List<T> b ){
    int n = a.length();
    T Eab_sum = 0;
    for(int i=0; i<n; ++i){
        Eab_sum += a[i]*b[i];
    }
    T suma = sum(a);
    T sumb = sum(b);
    return (Eab_sum - suma*sumb/n)/n;
}

template <typename T>
float stdev(const List<T> list){
    return std::sqrt(variance(list));
}


#endif