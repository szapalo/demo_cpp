
#ifndef NUNU_TRIPLE_H_
#define NUNU_TRIPLE_H_


template <typename X, typename Y, typename Z>
struct Triple{
    X x;
    Y y;
    Z z;
    Triple(X x, Y y, Z z):x(x),y(y),z(z){}
};



#endif