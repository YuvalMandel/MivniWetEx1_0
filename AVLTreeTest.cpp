/*
 * testAVLTree.cpp
 *
 *  Created on: 19 ���� 2013
 *      Author: shirel
 */

#include "AVL.h"
#include <vector>
#include <ostream>
#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <string>

template<class Value>
std::string printAVLNode(AVLNode<Value>* avl_node,
        int layer){

    if (avl_node != nullptr){
        std::string right_print = printAVLNode(avl_node -> right_son, layer +
        1);
        std::string left_print = printAVLNode(avl_node -> left_son, layer + 1);
        return "layer: " + std::to_string(layer) +
        " value: " + std::to_string (avl_node-> val)
        + "\n" + "left " + left_print + "right " + right_print;
    }

    return "NULL\n";
}

int main(){

	//Specific Cases

	/*correct output for the four next cases
	1 BF: 0 Height: 0
	2 BF: 0 Height: 1
	3 BF: 0 Height: 0 */

	//basic LL root change
    AVLTree<int> tree1;
	tree1.Insert(3);
	tree1.Insert(2);
	tree1.Insert(1);
    std::cout << "tree1\n" << printAVLNode(tree1.root, 0);

    AVLTree<int> tree2;
	//basic LR root change
    tree2.Insert(3);
    tree2.Insert(1);
    tree2.Insert(2);
    std::cout << "tree2\n" << printAVLNode(tree2.root, 0);

	//basic RR root change
    AVLTree<int> tree3;
    tree3.Insert(1);
    tree3.Insert(2);
    tree3.Insert(3);
    std::cout << "tree3\n" << printAVLNode(tree3.root, 0);

	//basic RL root change
    AVLTree<int> tree4;
    tree4.Insert(1);
    tree4.Insert(3);
    tree4.Insert(2);
    std::cout << "tree4\n" << printAVLNode(tree4.root, 0);
    
//
//	//basic LR not root change
//	/*correct output
//	1 BF: 0 Height: 0
//	2 BF: 0 Height: 1
//	3 BF: 0 Height: 0
//	4 BF: 1 Height: 2
//	5 BF: 0 Height: 0
//	6 BF: 1 Height: 3
//	7 BF: 0 Height: 0
//	8 BF: 1 Height: 1 */
//	tree.Insert(6,6);
//	tree.Insert(4,4);
//	tree.Insert(8,8);
//	tree.Insert(3,3);
//	tree.Insert(5,5);
//	tree.Insert(7,7);
//	tree.Insert(1,1);
//	tree.Insert(2,2);
//	tree.printTree();
//	tree.treeClear();
//
//	//basic LL not root change
//	/*correct output
//	1 BF: 0 Height: 0
//	2 BF: 0 Height: 1
//	3 BF: 0 Height: 0
//	4 BF: 1 Height: 2
//	5 BF: 0 Height: 0
//	6 BF: 1 Height: 3
//	7 BF: 0 Height: 0
//	8 BF: 1 Height: 1 */
//	tree.Insert(6,6);
//	tree.Insert(4,4);
//	tree.Insert(8,8);
//	tree.Insert(3,3);
//	tree.Insert(5,5);
//	tree.Insert(7,7);
//	tree.Insert(2,2);
//	tree.Insert(1,1);
//	tree.printTree();
//	tree.treeClear();
//
//	//basic RR not root change
//	/*correct output
//	 * 1 BF: 0 Height: 0
//	2 BF: 0 Height: 1
//	3 BF: 0 Height: 0
//	5 BF: 1 Height: 2
//	7 BF: 0 Height: 0*/
//	tree.Insert(5,5);
//	tree.Insert(7,7);
//	tree.Insert(1,1);
//	tree.Insert(2,2);
//	tree.Insert(3,3);
//	tree.printTree();
//	tree.treeClear();
//
//	//basic RL not root change
//	/*correct output
//	 * 1 BF: 0 Height: 0
//	2 BF: 0 Height: 1
//	3 BF: 0 Height: 0
//	5 BF: 1 Height: 2
//	7 BF: 0 Height: 0*/
//	tree.Insert(5,5);
//	tree.Insert(7,7);
//	tree.Insert(1,1);
//	tree.Insert(3,3);
//	tree.Insert(2,2);
//	tree.printTree();
//	tree.treeClear();
//
//	//root deletion no roll successor is a neighbour
//	/*correct output
//	2 BF: 0 Height: 0
//	3 BF: 1 Height: 1
//	5 BF: 1 Height: 2
//	6 BF: 0 Height: 0*/
//	tree.Insert(4,4);
//	tree.Insert(5,5);
//	tree.Insert(3,3);
//	tree.Insert(2,2);
//	tree.Insert(6,6);
//	tree.removeNode(4);
//	tree.printTree();
//	tree.treeClear();
//
//	//root deletion no roll successor is not a neighbour
//	/*correct output
//	2 BF: 0 Height: 0
//	3 BF: 1 Height: 1
//	5 BF: 0 Height: 2
//	6 BF: 0 Height: 0
//	7 BF: 0 Height: 1
//	8 BF: 0 Height: 0 */
//	tree.Insert(4,4);
//	tree.Insert(3,3);
//	tree.Insert(7,7);
//	tree.Insert(2,2);
//	tree.Insert(5,5);
//	tree.Insert(8,8);
//	tree.Insert(6,6);
//	tree.removeNode(4);
//	tree.printTree();
//	tree.treeClear();
//
//	//node deletion no roll successor is a neighbour case7
//	/*correct output
//	1 BF: 0 Height: 0
//	2 BF: 1 Height: 1
//	4 BF: 1 Height: 2
//	5 BF: 0 Height: 0
//	7 BF: 1 Height: 3
//	8 BF: -1 Height: 1
//	9 BF: 0 Height: 0*/
//	tree.Insert(7,7);
//	tree.Insert(3,3);
//	tree.Insert(8,8);
//	tree.Insert(2,2);
//	tree.Insert(4,4);
//	tree.Insert(9,9);
//	tree.Insert(5,5);
//	tree.Insert(1,1);
//	tree.removeNode(3);
//	tree.printTree();
//	tree.treeClear();
//
//	//node deletion no roll successor is not a neighbour case8
//	/*correct output
//	1 BF: 0 Height: 0
//	2 BF: 1 Height: 1
//	5 BF: 0 Height: 2
//	6 BF: 0 Height: 0
//	7 BF: 0 Height: 1
//	8 BF: 0 Height: 0
//	9 BF: 0 Height: 3
//	10 BF: 0 Height: 0
//	12 BF: -1 Height: 2
//	13 BF: -1 Height: 1
//	14 BF: 0 Height: 0*/
//	tree.Insert(9,9);
//	tree.Insert(3,3);
//	tree.Insert(12,12);
//	tree.Insert(2,2);
//	tree.Insert(7,7);
//	tree.Insert(10,10);
//	tree.Insert(13,13);
//	tree.Insert(1,1);
//	tree.Insert(5,5);
//	tree.Insert(8,8);
//	tree.Insert(14,14);
//	tree.Insert(6,6);
//	tree.removeNode(3);
//	tree.printTree();
//	tree.treeClear();
//
//	//node deletion causing LR case9
//	/*correct output
//	1 BF: 0 Height: 0
//	2 BF: 0 Height: 1
//	3 BF: 0 Height: 0
//	4 BF: 0 Height: 2
//	7 BF: -1 Height: 1
//	8 BF: 0 Height: 0*/
//	tree.Insert(7,7);
//	tree.Insert(2,2);
//	tree.Insert(8,8);
//	tree.Insert(1,1);
//	tree.Insert(4,4);
//	tree.Insert(9,9);
//	tree.Insert(3,3);
//	tree.removeNode(9);
//	tree.printTree();
//	tree.treeClear();
//
//	//node deletion causing LL case10
//	/*correct output
//	1 BF: 0 Height: 0
//	2 BF: 1 Height: 1
//	3 BF: 0 Height: 2
//	4 BF: 0 Height: 0
//	7 BF: 0 Height: 1
//	8 BF: 0 Height: 0*/
//	tree.Insert(7,7);
//	tree.Insert(3,3);
//	tree.Insert(8,8);
//	tree.Insert(2,2);
//	tree.Insert(4,4);
//	tree.Insert(9,9);
//	tree.Insert(1,1);
//	tree.removeNode(9);
//	tree.printTree();
//	tree.treeClear();
//
//	//node deletion causing RR case11
//	/*correct output
//	2 BF: 0 Height: 0
//	3 BF: 0 Height: 1
//	7 BF: 0 Height: 0
//	8 BF: 0 Height: 2
//	9 BF: -1 Height: 1
//	10 BF: 0 Height: 0*/
//	tree.Insert(3,3);
//	tree.Insert(2,2);
//	tree.Insert(8,8);
//	tree.Insert(7,7);
//	tree.Insert(1,1);
//	tree.Insert(9,9);
//	tree.Insert(10,10);
//	tree.removeNode(1);
//	tree.printTree();
//	tree.treeClear();
//
//	//node deletion causing RL case12
//	/*correct output
//	* 2 BF: 0 Height: 0
//	3 BF: 1 Height: 1
//	7 BF: 0 Height: 2
//	10 BF: 0 Height: 0
//	13 BF: 0 Height: 1
//	14 BF: 0 Height: 0*/
//	tree.Insert(3,3);
//	tree.Insert(2,2);
//	tree.Insert(13,13);
//	tree.Insert(7,7);
//	tree.Insert(1,1);
//	tree.Insert(14,14);
//	tree.Insert(10,10);
//	tree.removeNode(1);
//	tree.printTree();
//	tree.treeClear();
//
//	//double rotations RL and RR case 13
//	/*correct output
//	3 BF: 0 Height: 0
//	5 BF: 0 Height: 1
//	7 BF: 0 Height: 0
//	9 BF: 0 Height: 2
//	11 BF: 0 Height: 0
//	13 BF: 1 Height: 1
//	15 BF: 0 Height: 3
//	17 BF: 0 Height: 0
//	19 BF: -1 Height: 2
//	21 BF: -1 Height: 1
//	23 BF: 0 Height: 0*/
//	tree.Insert(9,9);
//	tree.Insert(3,3);
//	tree.Insert(15,15);
//	tree.Insert(1,1);
//	tree.Insert(7,7);
//	tree.Insert(13,13);
//	tree.Insert(19,19);
//	tree.Insert(5,5);
//	tree.Insert(11,11);
//	tree.Insert(17,17);
//	tree.Insert(21,21);
//	tree.Insert(23,23);
//	tree.removeNode(1);
//	tree.printTree();
//	tree.treeClear();
//
//	//double rotations RR and RR case 14
//	/*correct output
//	3 BF: 0 Height: 0
//	7 BF: 0 Height: 1
//	8 BF: 0 Height: 0
//	9 BF: 0 Height: 2
//	11 BF: 0 Height: 0
//	13 BF: 1 Height: 1
//	15 BF: 0 Height: 3
//	17 BF: 0 Height: 0
//	19 BF: -1 Height: 2
//	21 BF: -1 Height: 1
//	23 BF: 0 Height: 0*/
//	tree.Insert(9,9);
//	tree.Insert(3,3);
//	tree.Insert(15,15);
//	tree.Insert(1,1);
//	tree.Insert(7,7);
//	tree.Insert(13,13);
//	tree.Insert(19,19);
//	tree.Insert(8,8);
//	tree.Insert(11,11);
//	tree.Insert(17,17);
//	tree.Insert(21,21);
//	tree.Insert(23,23);
//	tree.removeNode(1);
//	tree.printTree();
//	tree.treeClear();
//
//	//double rotations RL and LL case 15
//	/*correct output
//	6 BF: 0 Height: 0
//	7 BF: 1 Height: 1
//	8 BF: 1 Height: 2
//	9 BF: 0 Height: 0
//	10 BF: 0 Height: 3
//	11 BF: -1 Height: 1
//	12 BF: 0 Height: 0
//	15 BF: 0 Height: 2
//	17 BF: 0 Height: 0
//	18 BF: 0 Height: 1
//	20 BF: 0 Height: 0
//	 */
//	tree.Insert(15,15);
//	tree.Insert(10,10);
//	tree.Insert(20,20);
//	tree.Insert(8,8);
//	tree.Insert(11,11);
//	tree.Insert(17,17);
//	tree.Insert(21,21);
//	tree.Insert(7,7);
//	tree.Insert(9,9);
//	tree.Insert(12,12);
//	tree.Insert(18,18);
//	tree.Insert(6,6);
//	tree.removeNode(21);
//	tree.printTree();
//	tree.treeClear();
//
//	//double rotations LR and LL case 16
//	/*correct output
//	6 BF: 0 Height: 0
//	7 BF: 1 Height: 1
//	8 BF: 1 Height: 2
//	9 BF: 0 Height: 0
//	10 BF: 0 Height: 3
//	11 BF: -1 Height: 1
//	12 BF: 0 Height: 0
//	15 BF: 0 Height: 2
//	20 BF: 0 Height: 0
//	21 BF: 0 Height: 1
//	22 BF: 0 Height: 0 */
//	tree.Insert(15,15);
//	tree.Insert(10,10);
//	tree.Insert(20,20);
//	tree.Insert(8,8);
//	tree.Insert(11,11);
//	tree.Insert(17,17);
//	tree.Insert(22,22);
//	tree.Insert(7,7);
//	tree.Insert(9,9);
//	tree.Insert(12,12);
//	tree.Insert(21,21);
//	tree.Insert(6,6);
//	tree.removeNode(17);
//	tree.printTree();
//	tree.treeClear();
//
//	//delete node cause LR
//	/*correct output
//	 * 2 BF: 0 Height: 0
//	3 BF: -1 Height: 2
//	4 BF: 0 Height: 0
//	6 BF: 1 Height: 1*/
//
//	tree.Insert(5,5);
//	tree.Insert(3,3);
//	tree.Insert(6,6);
//	tree.Insert(2,2);
//	tree.Insert(4,4);
//	tree.removeNode(5);
//	tree.printTree();
//	tree.treeClear();
//
//	//delete node cause LR
//	/*correct output
//	 * 2 BF: 0 Height: 0
//	3 BF: 0 Height: 1
//	6 BF: 0 Height: 0*/
//	tree.Insert(5,5);
//	tree.Insert(3,3);
//	tree.Insert(6,6);
//	tree.Insert(2,2);
//	tree.removeNode(5);
//	tree.printTree();
//	tree.treeClear();
//
//	std::vector<int> vector;
//	 for (int i=1; i<=100; i++) vector.push_back(i);
//
//	 //Randomly insert and removes nodes
//	 for (int k = 0; k < 20; ++k) {
//		 unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
//		 shuffle (vector.begin(), vector.end(), std::default_random_engine(seed));
//		 for (std::vector<int>::iterator it = vector.begin() ; it != vector.end(); ++it){
//				tree.Insert(*it,*it);
//
//		 }
//		 tree.printTree();
//		 shuffle (vector.begin(), vector.end(), std::default_random_engine(seed));
//		 for (std::vector<int>::iterator it = vector.begin() ; it != vector.end(); ++it){
//				tree.removeNode(*it);
//				tree.printTree();
//
//		 }
//		 tree.treeClear();
//		 tree.printTree();
//		 std::cout << '\n';
//	}

	return 0;
}

