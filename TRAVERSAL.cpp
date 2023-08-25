//  IN ORDER

class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        
        vector<int> result;
        stack<TreeNode*>  stk;
        TreeNode* curr_node = root;
        while(!stk.empty() || curr_node){

            while(curr_node){
                stk.push(curr_node);
                curr_node = curr_node->left;
            }

            curr_node = stk.top();
            stk.pop();
            
            result.push_back(curr_node->val);
            curr_node = curr_node->right;

        }
        return result;
    }
    
};


// preorder 
class Solution {
public:
    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> result;
        if(!root){
            return result;
        }
        stack<TreeNode*> stk;
        TreeNode* curr_node;
        stk.push(root);
        
        while(!stk.empty()){
            curr_node = stk.top();
            stk.pop();
            
            result.push_back(curr_node->val);
            
            if(curr_node->right){
                stk.push(curr_node->right);            
            }
            if(curr_node->left){
                stk.push(curr_node->left);            
            }

        }
        return result;
    }
};

// post order

class Solution {
public:
    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> result;
        if(!root){
            return result;
        }
        stack<TreeNode*> stk;
        set<TreeNode*> visited;
        
        TreeNode* curr_node = root;
        stk.push(root);
        
        while(!stk.empty()){
            curr_node = stk.top();
            
            if(visited.find(curr_node) == visited.end()){
                visited.insert(curr_node);
            }
            else{
                result.push_back(curr_node->val);
                stk.pop();
                continue;
            }
            if(curr_node->right){
                stk.push(curr_node->right);
            }
            
            if(curr_node->left){
                stk.push(curr_node->left);
            }

        }
        return result;
    }
};