#ifndef NUNU_LAYER_H_
#define NUNU_LAYER_H_

// #include <set>
#include "./node/Node.cpp"

class Layer {
    public:

    Container<Node*> nodes;

    int layer;

    Layer(int layer_num){
        layer = layer_num;
    }

    auto add_node(Node &node){
        return nodes.insert(&node);
    }

};

class Layers{
    public:
    
    vector<Layer*> layer_vec;

    Layers(){}

    void add_node(Node &node, int layer_num){
        if(layer_num >= layer_vec.size()){
            for(int i=layer_vec.size(); i<=layer_num; i++){
                layer_vec.push_back(new Layer(i));
            }
            (*layer_vec.back()).add_node(node);
        }
        else{
            (*layer_vec[layer_num]).add_node(node);
        }
    }
};


#endif