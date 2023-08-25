#ifndef NUNU_TRAVEL_H_
#define NUNU_TRAVEL_H_

#include <queue>
#include <stack>
#include <vector>

#include "./Network.cpp"
#include "./Layer.cpp"

#include <iostream>
using namespace std;



class Travel{
    public:

    Network* network;

    Travel(Network* network):network(network){}

    ~Travel(){
        delete network;
    }

    // verify cycles
    // find output_groups?
    // return bool?
    void dfs(Group &root_group){
        (*network).visited = true;
        // Stack of traversed groups
        stack<Group*> stk;
        // Stack of group's front_groups' iterator;
        stack<Group*> stk_it;

        stk.push(&root_group);
        stk_it.push(root_group.front_groups.begin());
        
        Group* current_group = stk.top();
        auto current_it = stk_it.top();
        
        while(!stk.empty()){

            if(current_it != (*current_group).front_groups.end()){
                Group* front_group = *current_it;
                // if next group is not visited at all
                if((*front_group).visited < 0){
                    // visit
                    (*front_group).visited = 0;

                    // increment iterator
                    stk.top()++;

                    current_group = front_group;
                    current_it = (*front_group).front_groups.begin();

                    stk.push(current_group);
                    stk_it.push(current_it);
                }
                // if current front group has been traversed,
                // and no cycle was found
                else if( (*front_group).visited > 0 ){
                    stk.top()++;
                }// if current front group was already visited by running dfs 
                else{
                    //Error: cycle is found!!
                    unvisit_all_groups();
                    throw runtime_error("Cycle is found on a Group!");
                    //  or return bool?
                }
            }
            else{
                
                (*current_group).visited = 1;

                stk.pop();
                stk_it.pop();

                current_group = stk.top();
                current_it = stk_it.top();
            }
        }
        unvisit_all_groups();

    }

    // verify there are no cycles
    void validate_graph(Container<Group*> &input_groups){
        for(auto group : input_groups){
            dfs(*group);
        }
        // unvisit_all_groups();
    }

    void unvisit_all_groups(){
        if(!(*network).visited){
            return;
        }
        for(auto group : (*network).all_groups){
            (*group).visited = -1;
        }
        (*network).visited = false;
    }

    // assign layer to each group
    void bfs(Container<Group*> input_groups){
        (*network).visited = true;

        queue<Group*> que;
        for(auto& group : input_groups){
            if((*group).layer < 0){
                throw runtime_error("Layer of input groups are not initialized");
            }
            que.push(group);
        }

        while(!que.empty()){
            Group* current_group = que.front();
            que.pop();

            int current_layer = (*current_group).layer;

            for(auto front_group : (*current_group).front_groups){
                // int front_layer = (*front_group).layer;
                if( current_layer >= (*front_group).layer ){
                    (*front_group).layer = current_layer+1;
                    que.push(front_group);
                }
            }
            que.pop();
        }
        unvisit_all_groups();
    }

    void capture_layers(Container<Group*> input_groups, Layer &layers){
        (*network).visited = true;

        queue<Group*> que;
        for(auto group : input_groups){
            (*group).visited = 0;
            que.push(group);
            do{
                Group* current_group = que.front();
                int current_layer = (*current_group).layer;
                if(current_layer < 0){
                    throw runtime_error("Group layer was not initialized.");
                }
                
                layers.add_group(*current_group, current_layer);

            }while(!que.empty());
        }
        unvisit_all_groups();
    }

};

int main(){

}
#endif