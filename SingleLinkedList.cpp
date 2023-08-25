
template <typename T>
class SingleLinkedList{
    private:
    class Node {
        public:
        T data;
        Node* next;
        Node(const T data):data(data){}
    };

    Node* head = nullptr;

    public:
    SingleLinkedList(){}
    ~SingleLinkedList(){}

    void clear(){
        while(head){
            pop_back();
        }
    }

    Node* find(const T& data){
        Node* curr_node = head;
        while(curr_node){
            if(curr_node->data == data){
                return curr_node;
            }
            curr_node = curr_node->next;
        }
        return nullptr;
    }

    void push_front(const T& data){
        Node* new_head = new Node(data);
        new_head->next = head;
        head = new_head;
    }

    void pop_front(const T& data){
        if(head){
            Node* old_head = head;
            head = head->next;
            old_head->~Node();
        }
    }

    void pop_back(){
        if(head){
            Node* prev_node = head;
            Node* curr_node = prev_node->next;
            if(!curr_node){
                head = nullptr;
            }
            while(curr_node->next){
                prev_node = curr_node;
                curr_node = curr_node->next;
            }
            curr_node->~Node();
            prev_node->next = nullptr;
        }
    }

};