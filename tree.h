#ifndef TREE_H
#define TREE_H
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;
extern const int GOAL_VALUE;

class Node {
	public:
		Node();
		int arr[9];
		int nodeVal;
		Node* parent; //points to parent node (node before)
		Node* leftMost; //each node can have at most 4 children, populate from 
		Node* left;		//leftMost -> rightMost
		Node* right;
		Node* rightMost;
		void addChild(Node*& newNode); //takes in a Node and adds it as a child in its according location
		int findBlank();
		int getG();
		int getH();
		int getF();
		void printPuzzleBoard();
	protected:
		void calcNodeVal();
		virtual void calcH() = 0;
		int g, h, f;
};

class UniformCostSearch : public Node {
	public:
		UniformCostSearch(int pb[], Node*& par);
		UniformCostSearch(int pb[]);
	private:
		void calcH();
};

class MisplacedTile : public Node {
	public:
		MisplacedTile(int pb[], Node*& par);
		MisplacedTile(int pb[]);
	private:
		void calcH();
};

class EucledeanDistance : public Node {
	public:
		EucledeanDistance(int pb[], Node*& par);
		EucledeanDistance(int pb[]);
	private:
		void findCoordinates(int& x, int& y, int index);
		void calcH();
};

class Tree {
	public:
		Tree(int arr[], int type);
		void buildTree();
		void expandNode(Node*& curNode);
	protected:
		/*These 3 insert functions makes sure the vector remains sorted from least to 
		greatest. In regard to the Frontier, the node with the smallest f(n) value
		is at the first item of the vector*/
		void insertFrontier(Node*& insertNode);
		void insertCreatedNodes(int targetVal);
		void insertNodeValues(int targetVal);
		unsigned int expandedNodesCount, maxFrontierSize, algorithmType;
		Node* root;
		/*Will recursively call this function on the cheapest goal state found
		to reach the root. Then will be returning down the tree path, outputting
		which nodes were expanded*/
		void printFinalSolution(Node*& curNode);
		bool inFrontier(int targetVal); //checks if targetVal is inside the frontier
		//bool inCreatedNodes(int left, int right, int targetVal);
		vector <int> nodeValues; //vector of all explored node values
		vector <Node*> frontier;
		vector <int> createdNodes; // vector of all nodes created, used to not create duplicates
		void addNode(int arr[], Node*& parent); //addNode checks to see if newNode already exists in tree 
};
#endif // !TREE_H
