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

    AVLNode(const Value& val):
    Value(val),
    right_son(nullptr),
    left_son(nullptr),
    height(1){};

};

template<class Value>
class AVLTree{

public:

    AVLTree() : root(nullptr){}
    AVLTree(Value*){} // TODO
    ~AVLTree() { DestroyTree(root);}

    AVLNode<Value>* root;

    void Insert(const Value& val);
    void Remove(const Value& val);
    AVLNode<Value>* FindValue(const Value& val);

private:

    void DestroyTree(AVLNode<Value>* root);
    AVLNode<Value>* FindValueInNode(const Value& val, AVLNode<Value>* node);
    AVLNode<Value>* InsertValueInNode(const Value& val, AVLNode<Value>* node);
    AVLNode<Value>* RemoveValueInNode(const Value& val, AVLNode<Value>* node);
    void UpdateHeight(AVLNode<Value>* node);
    AVLNode<Value>* BalanceNode(AVLNode<Value>* node);
    int CalcBalanceFactor(AVLNode<Value>* node);

    AVLNode<Value>* LLRotate(AVLNode<Value>* node);
    AVLNode<Value>* LRRotate(AVLNode<Value>* node);
    AVLNode<Value>* RLRotate(AVLNode<Value>* node);
    AVLNode<Value>* RRRotate(AVLNode<Value>* node);

};

#endif //MIVNIWETEX1_0_AVL_H
