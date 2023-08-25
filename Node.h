
#ifndef NUNU_NODE_H_
#define NUNU_NODE_H_

#include "List.h"
#include "Tensor.h"

#include <iostream>


class Node{

    private:

    public:
    int layer = -1;
    bool visited = false;
    bool in_branch = false; 

    int num_perceptrons = 0;
    List<Node*> front_nodes;

    Tensor<1>* feed_value;
    Tensor<1>* back_value; 

    List<Tensor<2>*> weights;

    Node(int num_perceptrons):num_perceptrons(num_perceptrons){
        // assert num_perceptrons > 0

        feed_value = new Tensor<1>(num_perceptrons);
        back_value = new Tensor<1>(num_perceptrons);

    }

    ~Node(){
    }

    void print(){
        // (*values).print();
    }
};

class Metanode{};

#endif
