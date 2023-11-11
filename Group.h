
#ifndef NUNU_GROUP_H_
#define NUNU_GROUP_H_

#include "./List.h"
#include "./Node.h"

// template <typename T>
class Group{
    public:

    List<Node*> nodes;

    // feedforward Groups
    List<Group*> front_groups;

    // backpropagation Groups
    List<Group*> back_groups;

    // traversal height in network
    int layer;

    // -1 == Not visited
    //  0 == Grey (i.e. visited by running search)
    //  1 == Black (i.e. visited with no-cycle confirmation) 
    short int visited = -1;

    // Group(int layer = -1){}

    Group(int num_nodes, int layer = -1){
        layer=layer;

        for (int i = 0; i < num_nodes; i++)
        {
            Node* n = new Node();
            nodes.append(n);
        }
    }

    Group(List<Node*> nodes, int layer=-1){
        nodes = nodes;
    }

    ~Group(){}

    // feedforward and packpropagation methods can be static
};


#endif