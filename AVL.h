//
// Created by Yuval.M on 04/12/2020.
//

#ifndef MIVNIWETEX1_0_AVL_H
#define MIVNIWETEX1_0_AVL_H

template<class Value>
class AVLNode{

public:
    Value val;
//    AVLNode* father;
    AVLNode<Value>* right_son;
    AVLNode<Value>* left_son;
    int height;

    AVLNode(const Value& val):val(val),right_son(nullptr),left_son(nullptr),
    height(1){};

};

template<class Value>
class AVLTree{

public:

    AVLTree() : root(nullptr){}
    AVLTree(Value* vals, int length) :root(createTreeNode(vals, 0, length-1)){}
    ~AVLTree() {DestroyTree(root);}
//    AVLTree(const AVLTree<Value>& avl_tree);
//    AVLTree<Value>& operator=(const AVLTree<Value>& avl_tree);

    AVLNode<Value>* root;

    Value* Insert(Value const& val);

    void Remove(const Value& val);
    AVLNode<Value>* FindValue(const Value& val);

private:

    AVLNode<Value>* createTreeNode(Value* vals, int start_index, int
    final_index);
    void DestroyTree(AVLNode<Value>* root);
    AVLNode<Value>* FindValueInNode(const Value& val, AVLNode<Value>* node);
    AVLNode<Value>* InsertValueInNode(const Value& val, AVLNode<Value>* node,
                                      Value** InsertedValPtr);
    AVLNode<Value>* RemoveValueInNode(const Value& val, AVLNode<Value>* node);
    void UpdateHeight(AVLNode<Value>* node);
    AVLNode<Value>* BalanceNode(AVLNode<Value>* node);
    int CalcBalanceFactor(AVLNode<Value>* node);

    AVLNode<Value>* LLRotate(AVLNode<Value>* node);
    AVLNode<Value>* LRRotate(AVLNode<Value>* node);
    AVLNode<Value>* RLRotate(AVLNode<Value>* node);
    AVLNode<Value>* RRRotate(AVLNode<Value>* node);

};

#include <algorithm>
#include <iostream>

//template<class Value>
//AVLTree<Value>::AVLTree(const AVLTree<Value>& avl_tree){
//    this->root=avl_tree.root;
//}
//
//template<class Value>
//AVLTree<Value>& AVLTree<Value>::operator=(const AVLTree<Value>& avl_tree){
//    this->root=avl_tree.root;
//}

template<class Value>
Value* AVLTree<Value>::Insert(const Value& val) {

    Value* temp = nullptr;
    this -> root = InsertValueInNode(val, this -> root, &temp);
    return temp;
}

template<class Value>
void AVLTree<Value>::Remove(const Value& val) {
    this -> root = RemoveValueInNode(val, this -> root);
}

template<class Value>
AVLNode<Value>* AVLTree<Value>::FindValue(const Value& val) {
    return FindValueInNode(val, this -> root);
}

template<class Value>
void AVLTree<Value>::DestroyTree(AVLNode<Value>* root){
    if(root != nullptr){
        DestroyTree(root ->right_son);
        DestroyTree(root ->left_son);
    }
    delete root;
}

template<class Value>
AVLNode<Value>* AVLTree<Value>::createTreeNode(Value* vals, int start_index,
                                               int final_index){
    if(final_index < start_index) return nullptr;

    AVLNode<Value>* base;

    int middle_index = start_index + (final_index - start_index)/2;

    try {
        base = new AVLNode<Value>(vals[middle_index]);
    }
    catch(std::bad_alloc&)
    {
        throw std::invalid_argument("ALLOCATION_ERROR");
    }

//    base -> val = vals[middle_index];
    if(start_index != final_index) {
        base->left_son = createTreeNode(vals, start_index, middle_index - 1);
        base->right_son = createTreeNode(vals, middle_index + 1, final_index);
    }
    return base;
}

template<class Value>
AVLNode<Value>* AVLTree<Value>::FindValueInNode(const Value& val, AVLNode<Value>* node) {
    if(node == nullptr){
        return nullptr;
    }
    else if(node -> val == val){
        return node;
    }
    else if (val < node -> val){
        return FindValueInNode(val, node -> left_son);
    }
    else if (val > node -> val){
        return FindValueInNode(val, node -> right_son);
    }

    return nullptr;

}

template<class Value>
AVLNode<Value>* AVLTree<Value>::InsertValueInNode(const Value& val,
                                                  AVLNode<Value>* node,
                                                  Value** InsertedValPtr) {
    if(node == nullptr){

        AVLNode<Value>* temp;

        try {
            temp = new AVLNode<Value>(val);
        }
        catch(std::bad_alloc&)
        {
            throw std::invalid_argument("ALLOCATION_ERROR");
        }

        *InsertedValPtr = &(temp -> val);
        return temp;
    }
    else if(node -> val == val){
        return nullptr; // TODO add exception
    }
    else if (val < node -> val){
        if(node -> left_son == nullptr){

            try {
                node -> left_son = new AVLNode<Value>(val);
            }
            catch(std::bad_alloc&)
            {
                throw std::invalid_argument("ALLOCATION_ERROR");
            }

            *InsertedValPtr = &(node -> left_son -> val);

        }
        else{
            node -> left_son = InsertValueInNode(val, node -> left_son, InsertedValPtr);
        }
    }
    else if (val > node -> val){
        if(node -> right_son == nullptr){

            try {
                node -> right_son = new AVLNode<Value>(val);
            }
            catch(std::bad_alloc&)
            {
                throw std::invalid_argument("ALLOCATION_ERROR");
            }

            *InsertedValPtr = &(node -> right_son -> val);

        }
        else{
            node -> right_son = InsertValueInNode(val, node -> right_son, InsertedValPtr);
        }
    }
    UpdateHeight(node);
    return BalanceNode(node);
}

template<class Value>
AVLNode<Value>* AVLTree<Value>::RemoveValueInNode(const Value &val,
                                                  AVLNode<Value> *node) {
    if(node == nullptr){
        return nullptr;
    }
    else if(val < node -> val){
        node -> left_son = RemoveValueInNode(val, node -> left_son);
        UpdateHeight(node);
        return BalanceNode(node);
    }
    else if(val > node -> val){
        node -> right_son = RemoveValueInNode(val, node -> right_son);
        UpdateHeight(node);
        return BalanceNode(node);
    }else{
        if(node -> right_son == nullptr && node -> left_son == nullptr){
            delete node;
            return nullptr;
        } else if(node -> right_son == nullptr){
            AVLNode<Value> *newNode = node -> left_son;
            delete node;
            return newNode;
        } else if(node -> left_son == nullptr){
            AVLNode<Value> *newNode = node -> right_son;
            delete node;
            return newNode;
        } else{

            // Go to the smallest node under the current node.
            AVLNode<Value> *newNode = node -> right_son;
            while (newNode -> left_son != nullptr){
                newNode = newNode -> left_son;
            }

            node -> right_son = RemoveValueInNode(
                    newNode -> val, node -> right_son);

            newNode -> left_son = node -> left_son;
            newNode -> right_son = node -> right_son;

            node -> left_son = nullptr;
            node -> right_son = nullptr;

            delete node;
            UpdateHeight(newNode);
            return BalanceNode(newNode);

        }
    }
}

template<class Value>
void AVLTree<Value>::UpdateHeight(AVLNode<Value>* node) {

    if(node == nullptr) return;

    int left_height;
    int right_height;

    if(node -> left_son != nullptr) {
        left_height = node->left_son->height;
    }else{
        left_height = 0;
    }

    if(node -> right_son != nullptr) {
        right_height = node->right_son->height;
    }else{
        right_height = 0;
    }

    node -> height = std::max(left_height, right_height) + 1;
}

template<class Value>
int AVLTree<Value>::CalcBalanceFactor(AVLNode<Value> *node) {

    if(node == nullptr) return 0;

    int left_height = (node -> left_son != nullptr) ?
                      node -> left_son -> height : 0;
    int right_height = (node -> right_son != nullptr) ?
                       node -> right_son -> height : 0;

    return left_height - right_height;

}

template<class Value>
AVLNode<Value>* AVLTree<Value>::BalanceNode(AVLNode<Value> *node) {

    int balance_factor = CalcBalanceFactor(node);

    if(balance_factor > 1){
        if(CalcBalanceFactor(node -> left_son) > 0){
            return LLRotate(node);
        } else {
            return LRRotate(node);
        }
    }else if(balance_factor < -1){
        if(CalcBalanceFactor(node -> right_son) > 0){
            return RLRotate(node);
        } else {
            return RRRotate(node);
        }
    }

    return node;

}

template<class Value>
AVLNode<Value>* AVLTree<Value>::LLRotate(AVLNode<Value> *node) {
    AVLNode<Value> *temp = node ->left_son;
    node ->left_son = temp -> right_son;
    temp -> right_son = node;
    UpdateHeight(node);
    UpdateHeight(temp);
    return temp;
}

template<class Value>
AVLNode<Value>* AVLTree<Value>::RRRotate(AVLNode<Value> *node) {
    AVLNode<Value> *temp = node ->right_son;
    node ->right_son = temp -> left_son;
    temp -> left_son = node;
    UpdateHeight(node);
    UpdateHeight(temp);
    return temp;
}

template<class Value>
AVLNode<Value>* AVLTree<Value>::LRRotate(AVLNode<Value> *node) {
    AVLNode<Value> *temp = node ->left_son;
    node -> left_son = RRRotate(temp);
    UpdateHeight(node);
    return LLRotate(node);
}

template<class Value>
AVLNode<Value>* AVLTree<Value>::RLRotate(AVLNode<Value> *node) {
    AVLNode<Value> *temp = node ->right_son;
    node -> right_son = LLRotate(temp);
    UpdateHeight(node);
    return RRRotate(node);
}



#endif //MIVNIWETEX1_0_AVL_H
