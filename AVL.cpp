//
// Created by Yuval.M on 04/12/2020.
//
#include "AVL.h"
#include <algorithm>

template<class Value>
void AVLTree<Value>::Insert(const Value& val) {
    InsertValueInNode(val, this -> root);
}

template<class Value>
void AVLTree<Value>::Remove(const Value& val) {
    RemoveValueInNode(val, this -> root);
}

template<class Value>
AVLNode<Value>* AVLTree<Value>::FindValue(const Value& val) {
    RemoveValueInNode(val, this -> root);
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
        return FindValueInNode(node -> left_son);
    }
    else if (val > node -> val){
        return FindValueInNode(node -> right_son);
    }
}

template<class Value>
AVLNode<Value>* AVLTree<Value>::InsertValueInNode(const Value& val, AVLNode<Value>* node) {
    if(node == nullptr){
        return nullptr;
    }
    else if(node -> val == val){
        return nullptr; // TODO add exception
    }
    else if (val < node -> val){
        if(node -> left_son == nullptr){
            node -> left_son = new AVLNode<Value>(val);
        }
        else{
            InsertValueInNode(val, node -> left_son);
        }
    }
    else if (val > node -> val){
        if(node -> right_son == nullptr){
            node -> right_son = new AVLNode<Value>(val);
        }
        else{
            insertValueInNode(val, node -> right_son);
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
    else if(node -> val == val){
        return nullptr; // TODO add exception
    }
    else if (val < node -> val){
        if(node -> left_son == nullptr){
            node -> left_son = new AVLNode<Value>(val);
        }
        else{
            InsertValueInNode(val, node -> left_son);
        }
    }
    else if (val > node -> val){
        if(node -> right_son == nullptr){
            node -> right_son = new AVLNode<Value>(val);
        }
        else{
            insertValueInNode(val, node -> right_son);
        }
    }
    UpdateHeight(node);
    return BalanceNode(node);
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

    if(node -> left_son != nullptr) {
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


