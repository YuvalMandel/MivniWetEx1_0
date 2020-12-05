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

            newNode -> left_son = node -> left_son;
            newNode -> right_son = node -> right_son;
            RemoveValueInNode(newNode -> val, node);

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


