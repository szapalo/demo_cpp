#ifndef NUNU_CONTAINER_H_
#define NUNU_CONTAINER_H_

// usleep
// #include <unistd.h>
#include <iostream>
#include "./List.h"
#include "./Deque.h"
#include "./Pair.h"

template <typename T, typename V=bool>
class Container{
    ////////////////////////////////////////////////////////////////////////////
    // Private
    ////////////////////////////////////////////////////////////////////////////
    private:
    int private_size = 0;

    class Redblack : public Pair<T,V>{
        friend class Container;
        private:
        Redblack* parent = NULL;
        bool red = true;
        Redblack* left = NULL;
        Redblack* right = NULL;

        public:

        Redblack(
            const T& key,
            Redblack* parent,
            const V& value=false
        ):Pair<T,V>{key,value},parent(parent){}
        // ~Redblack(){
        //     delete node;
        // }
    };

    Redblack* root_ptr = NULL;
    Redblack* end_ptr = NULL;


    Redblack* remove_swap(Redblack* rb){
       
        // find successor node of rb
        Redblack* succ = rb;
        T node; // can i do this???

        if(rb->right){

            // find node in the right subtree with minimum value
            succ = leftmost(rb->right);
            if(succ == rb->right){
                succ = rb->right;
                node = succ->key;

                rb->right = succ->right;
                
                // check if succ is the right most node
                if(succ->right){
                    succ->right->parent = rb;
                }
                else if(succ == end_ptr){
                    // assign parent as new rightmost node
                    end_ptr = rb;
                }

                /*
                    rb
                     \
                     succ
                     /  \
                   NULL  X
                */            
            }
            else{
                node = succ->key;

                succ->parent->left = succ->right;
                if(succ->right){
                    succ->right->parent = succ->parent;
                }

                /* 
                        rb
                         \
                         z
                        / \ 
                       .   x
                      / 
                    succ
                    /  \
                   NULL y
                */
            }
        }
        else if(rb->left){

            // find node in the left subtree with maximum value
            // here there is no right subtree of rb
            succ = rightmost(rb->left);
            if(succ==rb->left){
                succ = rb->left;
                node = succ->key;

                rb->left = succ->left;
                
                // check if succ is the right most node
                if(succ->left){
                    succ->left->parent = rb;
                }

                /*    
                      rb
                      /
                    succ
                    /  \
                   x  NULL
                */
            }
            else{
                node = succ->key;
                
                succ->parent->right = succ->left;
                if(succ->left){
                    succ->left->parent = succ->parent;
                }

                /*     
                    rb
                    /
                   z
                  / \ 
                 x   .
                      \ 
                     succ
                     /  \
                    y  NULL 
                */
            }
        }
        else{
            // if rb is leaf
            succ = rb;
            if(rb == end_ptr){
                end_ptr = rb->parent;
            }
            if(rb->parent){
                if(rb->parent->right == rb){
                    rb->parent->right = nullptr;
                }
                else{
                    rb->parent->left = nullptr;
                }
            }
            else{
                root_ptr = nullptr;
            }
        }
        rb->key = node;
        return succ;
    }

    static Redblack* leftmost(Redblack* rb){

        Redblack* current_rb = rb;
        while(current_rb->left){
            current_rb = current_rb->left;
        }
        return current_rb;
    }

    static Redblack* rightmost(Redblack* rb){

        Redblack* current_rb = rb;
        while(current_rb->right){
            current_rb = current_rb->right;
        }
        return current_rb;
    }

    // v == 'deleted' node , u = 'replaced' node 
    void remove_swivle( Redblack* u, Redblack* v){

        if(v->red || u->red){
            u->red = false;
            return;
        }
        if(!u->red && !v->red){

            bool double_black = true;
            Redblack* current_v = v;
            Redblack* new_root = nullptr;
            while(double_black){
                if(root_ptr == v){
                    return;
                }

                Redblack* parent = current_v->parent;
                if(!parent){break;}

                Redblack* s = nullptr; // sibling
                Redblack* r = nullptr; // child of sibling
                bool s_isleft = false;

                if(parent->left || parent->right){
                    if(parent->right && parent->right != current_v){
                        s = parent->right;
                    }
                    else if(parent->left && parent->left != current_v){
                        s = parent->left;
                        s_isleft = true;
                    }
                }
                
                if(!s){ return; }
                if(!s->red){
                    if(s->left &&  s->left->red){
                        r = s->left;
                        if(s_isleft){
                            // left left
                            zig(s, parent);
                            new_root = s;
                            // s->red = false;
                            r->red = false;
                        }
                        else{
                            // rigth left
                            r->red = false;

                            zigzag(r, s, parent);
                            new_root = r;

                        }
                    }
                    else if(s->right && s->right->red){
                        r = s->right;
                        if(s_isleft){
                            // left right
                            r->red = false;
                            zagzig(r, s, parent);
                            new_root = r;
                        }
                        else{
                            // rigth right
                            zag(s, parent);
                            r->red = false;
                            new_root = s;
                        }
                    }
                    else{
                        // all children of s are black
                        current_v = parent;
                        s->red = true;

                        if(current_v->red){
                            current_v->red = false;
                            double_black = false;
                            return;
                        }
                    }
                }
                else{
                    // is sibling is red
                    s->red = false;
                    parent->red = true;

                    if(s_isleft){
                        zig(s, parent);
                    }
                    else{
                        zag(s, parent);
                    }
                    new_root = s;
                }
                if(new_root && !new_root->parent){
                    root_ptr = new_root;
                }
            }
        }
    }

    void insert_swivle(Redblack* rb){
        Redblack* node_rb = rb;
        while(node_rb){ //why a while ? Try IF. NO! No touchy!
            node_rb = insert_zigzag(node_rb);
        }
    }

    Redblack* insert_zigzag(Redblack* rb){
        // http://pages.cs.wisc.edu/~skrentny/cs367-common/readings/Red-Black-Trees/
        
        Redblack* parent_rb = rb->parent;
        if(!parent_rb || !parent_rb->red){
            return nullptr;
        }
        
        Redblack* grand_parent = parent_rb->parent;
        if(!grand_parent){
            return nullptr;
        }

        bool isleft = false;
        if(parent_rb->left == rb){
            isleft = true;
        }
        
        Redblack* parent_sibling = nullptr;

        bool parent_isleft = false;
        if(grand_parent->left == parent_rb){
            parent_sibling = grand_parent->right;
            parent_isleft = true;
        }
        else{
            parent_sibling = grand_parent->left;
        }

        // If parent's sibling is black or null
        if(!parent_sibling || !parent_sibling->red){
            Redblack* new_root = nullptr;

            if(parent_isleft){
                if(isleft){
                    // zig
                    zig(parent_rb, grand_parent);
                    new_root = parent_rb;

                    parent_rb->red = false;
                    grand_parent->red = true;

                }
                else{
                    // zagzig
                    zagzig(rb, parent_rb, grand_parent);
                    new_root = rb;

                    rb->red = false;
                    grand_parent->red = true;
                }
            }
            else{
                if(isleft){
                    // zigzag
                    zigzag(rb, parent_rb, grand_parent);
                    new_root = rb;

                    rb->red = false;
                    grand_parent->red = true;
                }
                else{
                    // zag
                    zag(parent_rb, grand_parent);
                    new_root = parent_rb;

                    parent_rb->red = false;
                    grand_parent->red = true;
                }
            }
            
            if(!new_root->parent){
                root_ptr = new_root;
            }

            return nullptr;
        }
        // If parent's sibling is red
        else{
            // recoloring
            parent_rb->red = parent_sibling->red = false;
            grand_parent->red = true;
            return grand_parent;
        }

    }

    static void zig(Redblack* p, Redblack* gp){
        /* 
                gp              p
               /  \            / \
              p    u         x   gp
             / \                /  \    
            x  T1              T1   u
        */
        gp->left = p->right;
        if(p->right){
            p->right->parent = gp;
        }

        p->parent = gp->parent;
        if(gp->parent){
            if(gp->parent->left == gp){
                gp->parent->left = p;
            }
            else{
                gp->parent->right = p;
            }
        }
        gp->parent = p;
        p->right = gp;
    }

    static void zag(Redblack* p, Redblack* gp){
        /* 
                gp              p
               /  \            / \
              u    p         gp   x
                  / \       /  \    
                 T1  x     u    T1
        */
        gp->right = p->left;
        if(p->left){
            p->left->parent = gp;
        }

        p->parent = gp->parent;
        if(gp->parent){
            if(gp->parent->left == gp){
                gp->parent->left = p;
            }
            else{
                gp->parent->right = p;
            }
        }
        gp->parent = p;
        p->left = gp;
    }

    static void zagzig(Redblack* k, Redblack* p, Redblack* gp){
        /* 
                gp              x
               /  \           /   \
              p    u         p     gp
             / \            / \    / \
           T1   x         T1  T2  T3  u
               / \ 
             T2   T3
        */
        p->right = k->left;
        if(k->left){
            k->left->parent = p;
        }
        gp->left = k->right;
        if(k->right){
            k->right->parent = gp;
        }
        k->left = p;
        p->parent = k;

        k->parent = gp->parent;
        if(gp->parent){
            if(gp->parent->left == gp){
                gp->parent->left = k;
            }
            else{
                gp->parent->right = k;
            }
        }
        k->right = gp;
        gp->parent = k;
    }

    static void zigzag(Redblack* k, Redblack* p, Redblack* gp){
        /* 
                gp              x
               /  \           /   \
              u    p         gp    p
                  / \       / \   / \   
                 x  T1     u  T3 T2  T1
                / \ 
               T3   T2
        */
        p->left = k->right;
        if(k->right){
            k->right->parent = p;
        }
        gp->right = k->left;
        if(k->left){
            k->left->parent = gp;
        }
        k->right = p;
        p->parent = k;

        k->parent = gp->parent;
        if(gp->parent){
            if(gp->parent->left == gp){
                gp->parent->left = k;
            }
            else{
                gp->parent->right = k;
            }
        }
        k->left = gp;
        gp->parent = k;
    }

    ////////////////////////////////////////////////////////////////////////////
    // End Private
    ////////////////////////////////////////////////////////////////////////////
    // Public
    ////////////////////////////////////////////////////////////////////////////

    public:

    class Iterator{
        private:
        protected:
        Redblack* curr;
        List<Redblack*> s;
        public:

        Iterator():curr(nullptr){}

        Iterator(Redblack* root):curr(root){
            // curr = root;
            if (curr != NULL){
                while (curr !=  NULL)
                {
                    s.push(curr);
                    curr = curr->left;
                }
                curr = s.top();
                s.pop();
            }
        }

        Iterator(const Container<T,V>& container):Iterator(container.root_ptr){}

        void operator ++(){
            curr = curr->right;
            if ( curr || !s.empty() )
            {
                while (curr !=  NULL)
                {
                    s.push(curr);
                    curr = curr->left;
                }
                /* Current must be NULL at this point */
                curr = s.top();
                s.pop();
            }
        }

        const Redblack& operator*() const {
            return *curr;
        }

        const Redblack* operator->() const {
            return curr;
        }

        bool operator!=(const Iterator& it) const {
            return (it.curr != curr);
        }

        bool operator==(const Iterator& it) const {
            return (it.curr == curr);
        }
    };

    Container(){}

    Container( const Container<T,V>& container){
        for(Iterator it = container.begin(); it!=container.end(); ++it){
            insert((*it).key, (*it).value);
        }
    }

    const Container<T,V> operator=(const Container<T,V>& container) const{
        return Container<T,V>(container);
    }

    ~Container(){
        clear();
    }

    int size(){
        return private_size;
    }


    Redblack* find(const T& val) const{

        Redblack* current_node = root_ptr;
        while(current_node){
            if (current_node->key == val){return current_node;}
            if(val < current_node->key){
                current_node = current_node->left;
            }
            else{
                current_node = current_node->right;
            }
        }
        return current_node;
    }

    void print() const
    {
        List<Redblack*> s;
        if(!root_ptr){return;}
        Redblack*curr = root_ptr;

        while (curr != NULL || s.empty() == false)
        {
            /* Reach the left most Node of the
            curr Node */
            while (curr !=  NULL)
            {
                /* place pointer to a tree node on
                the stack before traversing
                the node's left subtree */
                s.push(curr);
                curr = curr->left;
            }
    
            /* Current must be NULL at this point */
            curr = s.top();
            s.pop();
    
            std::cout << curr->key << " ";

            /* we have visited the node and its
            left subtree.  Now, it's right
            subtree's turn */
            curr = curr->right;
    
        } /* end of while */
        std::cout<<std::endl;
    }

    Redblack* insert(const T& node, const V& value=V()){
        // if node is larger than the rightmost rebblack's node,
        // then append node as right most redblack node
        // if(node ==  nullptr){
        //     return nullptr;
        // }
        Redblack* this_end = end_ptr;

        // if the new node it larger than end-node
        if( this_end && node > this_end->key ){
            
            // insert at the rightmost end of the tree
            
            Redblack* rb = new Redblack{node, this_end, value};
            this_end->right = rb;
            end_ptr = rb;
            // reorder tree
            insert_swivle(rb);
            private_size++;
            return rb;
        }

        // otherwise traverse the tree to insert the new node 
        Redblack* current_rb = root_ptr;
        Redblack* parent_rb = nullptr;
        bool child_is_left = false;

        while(current_rb){
            parent_rb = current_rb;
            
            if(node > current_rb->key){
                current_rb = current_rb->right;
                child_is_left = false;
            }
            else if (node < current_rb->key){
                current_rb = current_rb->left;
                child_is_left = true;
            }
            else{
                // in case the node already exists
                return nullptr;
            }
        }

        Redblack* rb = new Redblack(node, parent_rb, value);

        // if parent of found leaf current rb is not null
        if(parent_rb){
            // assign parent of new node rb
            if(child_is_left){
                parent_rb->left = rb;
            }
            else{
                parent_rb->right = rb;
            }
        }
        else{
            // otherwise, rb is the root 
            root_ptr = rb;
            end_ptr = rb;
            rb->red = false;
        }

        // reorder tree
        insert_swivle(rb);
        private_size++;
        return rb;
    }

    bool remove(T node){
        // if(!node){
        //     return true;
        // }
        Redblack* current_rb = root_ptr;

        while(current_rb){
            if(node == current_rb->key){
                // remove
                // std::cout<<"xx"<<std::endl;
                // std::cout<<"a"<<std::endl;
                Redblack* succ = remove_swap(current_rb);
                remove_swivle(succ, current_rb);
                delete succ;
                private_size--;
                return true;
            }

            else if(node > current_rb->key){
                current_rb = current_rb->right;
            }
            else{
                current_rb = current_rb->left;
            }
        }
        return false;

    }

    void clear(){
        List<Redblack* > stk;
        Redblack* current_rb;
        if(!root_ptr){
            return;
        }
        stk.push(root_ptr);
        while(!stk.empty()){
            current_rb = stk.top();
            stk.pop();
            if(current_rb->left){
                stk.push(current_rb->left);
                current_rb->left = nullptr;
            }
            if(current_rb->right){
                stk.push(current_rb->right);
                current_rb->right = nullptr;
            }
            current_rb->parent = nullptr;
            delete current_rb;

        }
        private_size = 0;
        root_ptr = nullptr;
        end_ptr = nullptr;
    }

    const Redblack* root() const{
        return root_ptr;
    }

    const Iterator begin() const {
        return Iterator(root_ptr);
    }

    const Iterator end() const {
        return Iterator();
    }

    bool validate(){
        std::cout<<"validate"<<std::endl;
        std::cout<<root_ptr<<std::endl;

        Redblack* r = root_ptr;
        if(r && size()==0){
            std::cout<<"'empty' tree has non-zero size"<<std::endl;
        }
        else if(!r && size()==0){
            return true;
        }
        if(r->parent){
            std::cout<<"root has a parent"<<std::endl;
            return false;
        }
        else{
            return valid(r->left, r, true) && valid(r->right, r, false);
        }
    }

    static bool valid(Redblack* x, Redblack* p, bool isleft){
        if(!x){
            return true;
        }
        else if(!x->key){
            std::cout<<"redblack node has empty node element"<<std::endl;
            // is it a leaf?
            if(!x->left && !x->right){
                std::cout<<"it is a leaf by the way!!!"<<std::endl;       
            }
            return false;
        }
        else{
            if(x->parent == p ){
                if(isleft && x->key >= p->key){
                    std::cout<<"left child is not smaller than parent"<<std::endl;
                    return false;
                }
                else if(!isleft && x->key <= p->key){
                    std::cout<<"right child is not larger than parent"<<std::endl;
                    
                    return false;
                }
                return valid(x->left, x, true) && valid(x->right, x, false);
            }
            else{
                std::cout<<"child does not link parent"<<std::endl;
                return false;
            }
        }
    }

    

    static Redblack* isin__(Redblack* root, T x){
        if(!root){
            return nullptr;
        }
        else if(root->key == x){
            std::cout<<"it is found ==>"<<root<<std::endl;
            if(!root->left && !root->left){
                std::cout<<"it is a leaf"<<std::endl;
            }
            else{
                std::cout<<"NOT a leaf"<<std::endl;            
            }
            
            return root;
        }
        else{
            Redblack* left = isin( root->left, x);
            Redblack* right = isin(root->right, x);
            if(left){
                return left;
            }
            else if(right){
                return right;
            }
            else{
                return nullptr;
            }
        }
    }

    static int count(Redblack* td){
        if(td){
            return 1 + count(td->left) + count(td->right);
        }
        else{
            return 0;
        }
    }

};


#endif