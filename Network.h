
#ifndef NUNU_NETWORK_H_
#define NUNU_NETWORK_H_

// #include "./Builder.cpp"
// #include "./Travel.cpp"
#include "Group.cpp"
#include "Container.cpp"

#include <iostream>
// using namespace std;


class Network{
    public:

    // Builder builder;
    // Travel traveler;

    List<Group*>* input_groups;
    List<Group*>* output_groups;

    // Group root;

    Container<Group*> all_groups;
    Container<Group*> hidden_groups;

    bool visited = false;

    Group* get_input_group(int idx=0){
        return (*input_groups)[idx];
    }

    Network(int num_input_nodes){

        List<Group*>* og = new List<Group*>();
        output_groups = og;

        List<Group*>* ig = new List<Group*>();
        input_groups = ig;

        Group* input_group = new Group(num_input_nodes);
        input_groups->append(input_group);
    }

    ~Network(){
        for(auto group : *input_groups){
            delete group;
        }
    }

};


#endif