
#ifndef NUNU_GRAPH_H_
#define NUNU_GRAPH_H_

#include <iostream>
#include <sstream>

#include "./Map.h"
#include "./Set.h"
#include "./Pair.h"
#include "./List.h"
#include "./Deque.h"
#include "./Heap.h"

template<typename NodeType>
class GraphFunctor{
    public:
    virtual void operator()(NodeType* node, NodeType* from_node) = 0;
};


namespace {
    // typename Default = NULL;
    template <typename A, typename B>
    struct GraphPair {
        A first;
        B second;
        GraphPair(A f, B s):first(f),second(s){}
    };
    
};

typedef unsigned long ulong;

//////////////////////////// ADD HEURISTIC /////////////////////////////////////





////////////////////////////////////////////////////////////////////////////////

//////////// NODES /////////////////////////////////////////////////////////////
template<typename NodeID>
class BaseNode {
    private:
    public:
    NodeID id;
    Set<BaseNode<NodeID>*> nodes;
    BaseNode(NodeID id):id(id){}
    BaseNode(){};
};


template<typename NodeType, typename Value=bool>
class BaseEdge : public virtual GraphPair<NodeType*, NodeType*>{
    private:
    virtual void _assign_edges(NodeType* n1, NodeType* n2){
        n1->edges[n2->id] = this;
    }
    public:
    Value value;

    BaseEdge(
        NodeType* n1, NodeType* n2
    ) : GraphPair<NodeType*,NodeType*>(n1,n2){
        _assign_edges(n1,n2);
    }

    BaseEdge(){}

    NodeType* get_neighbor(NodeType* node){
        switch(node){
            case this->first:
                return this->second;
            case this->second:
                return this->first;
            default:
                return nullptr;
        }
    }
};

template<typename NodeID>
class NodeEdge : public virtual BaseNode<NodeID> {
    private:
    public:
    Map<NodeID, BaseEdge<NodeEdge<NodeID>>*> edges;
    Set<NodeEdge<NodeID>*> nodes;

    NodeEdge(NodeID id) : BaseNode<NodeID>(id){}
    NodeEdge(){};

    virtual void connect_node(NodeEdge<NodeID>* new_node){
        nodes.insert(new_node);
        BaseEdge<NodeEdge<NodeID>>* new_edge = new BaseEdge<NodeEdge<NodeID>>(
            this, new_node
        ); 
    }

    // EdgeType* get_edge(NodeType* node){
    // }
    // Node get_neighbor(EdgeType* edge){
    //     if(edges.find(rd)){}
    // }
};


template<typename NodeID, typename Value>
class BaseNodeEncap : public virtual BaseNode<NodeID>{
    private:
    public:
    Set<BaseNodeEncap<NodeID,Value>*> nodes;
    Value value;
    BaseNodeEncap(NodeID id, Value value): BaseNode<NodeID>(id),value(value){} 
    BaseNodeEncap(NodeID id): BaseNode<NodeID>(id){}
    BaseNodeEncap(){}
};


template<typename NodeID, typename Value>
class BaseNodeEncapEdge : 
    public virtual BaseNodeEncap<NodeID, Value>, 
    public virtual NodeEdge<NodeID>
{
    private:
    public:
    // Map<NodeID, BaseEdge<NodeEdge<NodeID>>*> edges; //!!!!
    Set<BaseNodeEncapEdge<NodeID,Value>*> nodes;
    BaseNodeEncapEdge(NodeID id, Value value): BaseNode<NodeID>(id){
        this->value = value;
    } 
    BaseNodeEncapEdge(NodeID id): BaseNode<NodeID>(id){}
    BaseNodeEncapEdge(){}
};


template<typename NodeID, typename NodeValue, typename EdgeValue>
class BaseNodeEncapEdgeEncap : public virtual BaseNodeEncapEdge<NodeID,NodeValue> {
    private:
    public:
    Map<
        NodeID,
        BaseEdge<
            BaseNodeEncapEdgeEncap<
                NodeID, NodeValue, EdgeValue
            >,
            EdgeValue
        >*
    > edges;

    Set<BaseNodeEncapEdgeEncap<NodeID,NodeValue,EdgeValue>*> nodes;
    BaseNodeEncapEdgeEncap(NodeID id, NodeValue value): BaseNode<NodeID>(id){
        this->value = value;
    }
    BaseNodeEncapEdgeEncap(NodeID id): BaseNode<NodeID>(id){}

    virtual void connect_node(
        BaseNodeEncapEdgeEncap<NodeID,NodeValue,EdgeValue>* new_node
    ){
        nodes.insert(new_node);
        BaseEdge<
            BaseNodeEncapEdgeEncap<NodeID, NodeValue, EdgeValue>,
            EdgeValue
        >* new_edge = new BaseEdge<
            BaseNodeEncapEdgeEncap<NodeID, NodeValue, EdgeValue>,EdgeValue
        >(this, new_node); 
    }
};

template<typename NodeType>
class BidirectionalEdge : public BaseEdge<NodeType>{
    private:
    public:

    BidirectionalEdge(NodeType* n1, NodeType* n2): BaseEdge<NodeType>(n1,n2){
        n2->edges[n1->id] = this;
    }
};

template<typename NodeType, typename Value>
class BidirectionalEdgeEncap : public BidirectionalEdge<NodeType>{
    private:
    public:
    Value value;
    
    BidirectionalEdgeEncap(NodeType* n1, NodeType* n2, Value value)
    : BidirectionalEdge<NodeType>(n1,n2), value(value){}
    
    BidirectionalEdgeEncap(
        NodeType* n1, NodeType* n2
    ) : BidirectionalEdge<NodeType>(n1,n2){}
};

// /////////////////////////////////////////////////////////////////////////////

// ////////////////// GRAPH ////////////////////////////////////////////////////
template<typename NodeType, typename NodeID>
class BaseNodeGraph {
    protected:

    virtual NodeType* _add_node(const NodeID& node_id) {
        auto id_node_pair = node_map.find(node_id);
        NodeType* node_ptr;
        if(!id_node_pair){
            node_ptr = new NodeType(node_id);
            node_map[node_id] = node_ptr;
        }
        else{
            node_ptr = id_node_pair->value;
        }
        return node_ptr; 
    }

    virtual void _add_edge(NodeType* first_node, NodeType* second_node){
        first_node->nodes.insert(second_node);
    }

    virtual void _init_(const List<Pair<NodeID,NodeID>>& init_list){
        auto it = init_list.begin();
        auto end_it = init_list.end();
        NodeID first_id;
        NodeID second_id;
        NodeType* first_node;
        NodeType* second_node;
        
        while(it != end_it) {
            first_id = it->key;
            second_id = it->value;
            first_node = _add_node(first_id);
            second_node = _add_node(second_id);
            _add_edge(first_node, second_node);
            ++it;
        }
    }

    public:
    Map<NodeID, NodeType*> node_map;
    using Node = NodeType;

    BaseNodeGraph(const List<Pair<NodeID,NodeID>>& init_list){
        _init_(init_list);
    }

    BaseNodeGraph(){}

    // virtual void add_edge(NodeID node_id1, NodeID node_id2){

    // }

    virtual NodeType& get_node(const NodeID& node_id) const {
        auto node_map_pair = this->node_map.find(node_id); 
        if(!node_map_pair){
            std::ostringstream buffer;
            buffer<<"Node "<<node_id<<" does not exist.";
            throw std::runtime_error(buffer.str());
        }
        return *(node_map_pair->value);
    }

    void connect_node(const NodeID& node_id, const NodeID& new_node_id){
        NodeType& node = get_node(node_id);
        NodeType* new_node = _add_node(new_node_id);
        _add_edge(&node, new_node);    
    }

    template <typename F>
    void dfs_preorder(const NodeID& root_node_id, F&& function){
        Set<NodeID> visited;
        visited.insert(root_node_id);
        NodeType* current_ptr;
        NodeType* previous_ptr=nullptr;

        List<NodeType*> stk;
        stk.push(&get_node(root_node_id));
        // std::cout<<"PREORDER" <<std::endl;
        while(!stk.empty()){

            current_ptr = stk.top();
            stk.pop();

            // Run Callback Function
            function(current_ptr, previous_ptr);
            previous_ptr = current_ptr;

            for(auto& elem : current_ptr->nodes){
                if(!visited.find(elem->id)){
                    stk.push(elem);
                    visited.insert(elem->id);
                }
            }
        }
    }

    template <typename F>
    void dfs_postorder(const NodeID& root_node_id, F&& function){ // in order
        // Set of visited nodes 
        // We mark a node as visited once it was added to stk.
        Set<NodeID> visited;
        // Stack of nodes
        List<NodeType*> stk;
        NodeType* current_ptr = &get_node(root_node_id);
        NodeType* previous_ptr = nullptr;
        stk.push(current_ptr);

        while(!stk.empty()){
            current_ptr = stk.top();
            if(!(visited.find(current_ptr->id))){
                visited.insert(current_ptr->id);
            }
            else{
                // Run Callback Function
                function(current_ptr, previous_ptr);
                previous_ptr = current_ptr;

                stk.pop();
                continue;
            }
            for(auto&& node_ptr : current_ptr->nodes){
                if(!(visited.find(node_ptr->id))){
                    stk.push(node_ptr);
                }
            }
        }
    }

    template<typename F>
    void breadth_first_search(const NodeID& root_node_id, F&& function){
        //Set of visited nodes 
        // We mark a node as visited once it was added to que.
        Set<NodeID> visited;
        visited.insert(root_node_id);
        // Queue of nodes
        Deque<NodeType*> que;
        NodeType* current_ptr;
        NodeType* previous_ptr=nullptr;

        que.push_back(&get_node(root_node_id));

        while(!que.empty()){
            current_ptr = que.front();
            que.pop_front();

            // Run Callback Function 
            function(current_ptr, previous_ptr);
            previous_ptr=current_ptr;

            for(auto& elem : current_ptr->nodes){
                if(!visited.find(elem->id)){
                    que.push_back(elem);
                    visited.insert(elem->id);
                }
            }
        }
    }

    Map<NodeID,int> stack(const NodeID& root_node_id){
        Map<NodeID, int> depth_map;
        depth_map[root_node_id] = 0;
        // Queue of nodes
        List<NodeType*> que;
        NodeType* current_ptr;
        int current_depth;
        int current_max_depth;
        que.push(&get_node(root_node_id));

        while(!que.empty()){
            current_ptr = que.top();
            current_depth = depth_map[current_ptr->id];
            que.pop();
            current_max_depth = 0;
            // do something
            std::cout<<current_ptr->id<<std::endl;
            //
            for(auto& elem : current_ptr->nodes){
                if(
                    !depth_map.find(elem->id)
                    || depth_map[elem->id] < current_depth + 1
                ){
                    que.push_back(elem);
                    depth_map[elem->id] = current_depth+1;
                }
            }
        }
        return depth_map;
    }

};

template<typename NodeType, typename EdgeType, typename NodeID>
class BaseNodeEdgeGraph : public virtual BaseNodeGraph<NodeType, NodeID>{
    protected:
    
    virtual void _add_edge(
        NodeType* first_node, NodeType* second_node
    ) override {
        BaseNodeGraph<NodeType, NodeID>::_add_edge(first_node, second_node);
        EdgeType* new_edge = new EdgeType(first_node, second_node);
        edge_map[first_node->id][second_node->id] = new_edge;
    }

    public:
    Map<NodeID, Map<NodeID, EdgeType*>> edge_map;
    BaseNodeEdgeGraph(const List<Pair<NodeID,NodeID>>& init_list){
        BaseNodeGraph<NodeType, NodeID>::_init_(init_list);
    }
    BaseNodeEdgeGraph(){}
};


template<typename NodeType, typename NodeID, typename Value>
class BaseNodeEncapGraph : public virtual BaseNodeGraph<NodeType, NodeID>{
    protected:
    virtual NodeType* _add_node(const NodeID& node_id) override {

        auto id_node_pair = this->node_map.find(node_id);
        if(!id_node_pair){
            throw std::runtime_error("Node has not been defined.");
        }
        return id_node_pair->value;
    }

    void _populate_nodes(const Map<NodeID,Value>& init_nodes){
        for(auto& elem : init_nodes){
            NodeID node_id = elem.key;
            Value value = elem.value;
            NodeType* new_node = new NodeType(node_id, value);
            this->node_map[node_id] = new_node;
        }
    }

    public:

    BaseNodeEncapGraph(
        const Map<NodeID,Value>& init_nodes,
        const List<Pair<NodeID,NodeID>>& init_list
    ){
        _populate_nodes(init_nodes);
        BaseNodeGraph<NodeType, NodeID>::_init_(init_list);
    }
    BaseNodeEncapGraph():BaseNodeGraph<NodeType, NodeID>(){}
};


template<
    typename NodeType, typename EdgeType, typename NodeID, 
    typename NodeValue, typename EdgeValue
>
class BaseNodeEncapEdgeEncapGraph :
        public virtual BaseNodeEdgeGraph<NodeType, EdgeType, NodeID>,
        public virtual BaseNodeEncapGraph<NodeType, NodeID, NodeValue> { 
    protected:
    void _assign_edge_values(
        const Map<Pair<NodeID,NodeID>, EdgeValue>& init_list
    ){
        for(auto& elem : init_list){
            auto node_id_pair = elem.key;
            EdgeValue edge_value = elem.value;
            this->edge_map[
                node_id_pair.key
            ][node_id_pair.value]->value = edge_value;
        }
    }

    public:
    using Edge = EdgeType;

    BaseNodeEncapEdgeEncapGraph(){}
    BaseNodeEncapEdgeEncapGraph(
        const Map<NodeID,NodeValue>& init_nodes,
        const Map<Pair<NodeID,NodeID>,EdgeValue>& init_list
    ){
        this->_populate_nodes(init_nodes);
        List<Pair<NodeID,NodeID>> pair_list = init_list.keys();
        BaseNodeEncapGraph<NodeType, NodeID, NodeValue>::_init_(pair_list);
        _assign_edge_values(init_list);
    }
};


////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// CONCRETE ///////////////////////////////////////

template<typename NodeID=int>
using NodeGraph = BaseNodeGraph<BaseNode<NodeID>, NodeID>;

template<typename NodeID=int>
using NodeEdgeGraph = BaseNodeEdgeGraph<
    NodeEdge<NodeID>,BaseEdge<NodeEdge<NodeID>>,NodeID
>;

template<typename V=int, typename NodeID=int>
using NodeEncapEdgeGraph = BaseNodeEncapGraph<
    BaseNodeEncap<NodeID,V>, NodeID, V
>;

template<typename V=int, typename E=int, typename NodeID=int>
using Graph = BaseNodeEncapEdgeEncapGraph<
                BaseNodeEncapEdgeEncap<NodeID,V,E>, 
                BaseEdge<BaseNodeEncapEdgeEncap<NodeID,V,E>,NodeID
              >, NodeID, V, E>;


#endif