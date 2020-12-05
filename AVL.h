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
    ~AVLTree() { DestroyTree(root);}

    AVLNode<Value>* root;

    void Insert(Value const& val);

    void Remove(const Value& val);
    AVLNode<Value>* FindValue(const Value& val);

private:

    AVLNode<Value>* createTreeNode(Value* vals, int start_index, int
    final_index);
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

#include "AVL.cpp"

#endif //MIVNIWETEX1_0_AVL_H
