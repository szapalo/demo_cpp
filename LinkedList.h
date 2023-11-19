
#ifndef NUNU_LINKEDLIST_H_
#define NUNU_LINKEDLIST_H_
// TODO: IMPLEMENT unsigned int length()
#include <iostream>

namespace DefaultComparators{

    template<typename T, typename V=T>
    class Equator{
        public:
        static bool equal(const T& lhs, const V& rhs) {
            std::cout<<"equal ? : "<<lhs << " :: "<<rhs<<std::endl;
            return ( lhs == rhs );
        }
    };
}


template <
    typename K, typename T=K,
    typename Comparator = DefaultComparators::Equator<K,T>
>
class BaseLinkedList {
    /*
                 ---next--> 
     (front) HEAD  ......  TAIL (back)    
                 <--prev--
     */
    public:
    class Node;

    private:
    // unsigned int size = 0; //should this be implemented?
    Node* head = nullptr; // front, contains prev data
    Node* tail = nullptr; // back, contains next data

    public:
    
    class Node {
        public:
        T data; 
        Node* next = nullptr;
        Node* prev = nullptr;
        Node(const T data):data(data){}
    };
    
    class Iterator { //: public AbsIterator<T>{ TODO
        public:
        Node* curr;
        Iterator(Node* node):curr(node){}
        void operator++(){
            curr = curr->next;
        }
        bool operator!=(const Iterator& it) const {
            return (it.curr != curr);
        }
        bool operator==(const Iterator& it) const {
            return (it.curr == curr);
        }
        const T& operator*(){
            return curr->data;
        }
        const T* operator->(){
            return &curr;
        }
    };

    BaseLinkedList(){}

    ~BaseLinkedList(){
        clear();
    }

    void clear(){ // this can be improved . 
        while(head){
            pop_back();
        }
    }

    void push_back_node(Node* node){
        if(tail){
            tail->next = node;
        }
        else{
            head = node;
        }
        node->prev = tail;
        tail = node;
    }

    void push_back(const T& data){
        Node* new_tail = new Node(data);
        push_back_node(new_tail);
    }

    void push_front_node(Node* node){
        if(head){
            head->prev = node;
        }
        else{
            tail = node;
        }
        node->next = head;
        head = node;
    }

    void push_front(const T& data){
        Node* new_head = new Node(data);
        push_front_node(new_head);
    }

    bool empty() const{
        return head;
    }

    void pop_front(){
        if(head){
            Node* old_head = head;
            head = head->next;
            if(head){
                head->prev = nullptr;
            }
            else{
                tail = nullptr;
            }
            delete old_head;
        }
    }

    void pop_back(){
        if(tail){
            Node* old_tail = tail;
            tail = tail->prev;
            if(tail){
                tail->next = nullptr;
            }
            else{
                head = nullptr;
            }
            delete old_tail;
        }
    }

    T& front() const {
        return head->data;
    }

    T& back() const {
        return tail->data;
    }

    Node* get_head() const {
        return head;
    }

    Node* get_tail() const {
        return tail;
    }

    Node* find(const K& k) const{
        Node* curr_node = head;
        while(curr_node){
            if(Comparator::equal(curr_node->data, k)){
                return curr_node;
            }
            curr_node = curr_node->next;
        }
        return nullptr;
    }

    Node* remove_node(Node* node, bool deallocate=true){
        Node* node_next = node->next;
        Node* node_prev = node->prev;
        if(node_next){
            node_next->prev = node_prev;
        }
        else{
            tail = node_prev;
        }
        if(node_prev){
            node_prev->next = node_next;
        }
        else{
            head = node_next;
        }
        if(deallocate){delete node;}
        else{
            node->next = nullptr;
            node->prev = nullptr; 
        }
        return node_next;
    }

    static void insert_after(Node& node, const T& data){
        Node* node_next = node->next;
        Node* node_prev = node->prev;
        Node* new_node = new Node(data);
        new_node->next = node_next;
        new_node->prev = node_prev;
        if(node_next){
            node_next->prev = new_node;
        }
        if(node_prev){
            node_prev->next = new_node;
        }
    }

    Iterator begin() const {
        return Iterator(head);
    }
    Iterator end() const {
        return Iterator(nullptr);
    }

    void print(){
        Node* curr_node = head;
        while(curr_node){
            std::cout<<"|"<<curr_node->data<<"|";
            if(curr_node->next){std::cout<<"<->";}
            curr_node = curr_node->next;
        }
        std::cout<<std::endl;
    }
};

template <typename T>
using LinkedList = BaseLinkedList<T, T>;

#endif