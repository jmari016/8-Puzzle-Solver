#include "tree.h"
const int GOAL_VALUE = 123456780;

Node::Node() {}

void Node::addChild(Node*& newNode) {
	if (this->leftMost == NULL)
		this->leftMost = newNode;
	else if (this->left == NULL)
		this->left = newNode;
	else if (this->right == NULL)
		this->right = newNode;
	else
		this->rightMost = newNode;
}

int Node::findBlank() {
	for (unsigned int i = 0; i < 9; ++i) {
		if (this->arr[i] == 0)
			return i;
	}
}

/*nodeVal is the order of the array as a single int. We do this to make it
easier to compare when looking for existing nodes. 
Simple example: arr[1, 2, 3, 4, 5, 6, 7, 8, 0] 
				nodeVal = 1234567890*/
void Node::calcNodeVal() { 
	int bitLength = 100000000;
	for (unsigned int i = 0; i < 9; ++i) {
		this->nodeVal += this->arr[i] * bitLength;
		bitLength /= 10;
	}
}

int Node::getG() {
	return this->g;
}

int Node::getH() {
	return this->h;
}

int Node::getF() {
	return this->f;
}

void Node::printPuzzleBoard() {
	for (unsigned int i = 0; i < 7; i += 3)
		cout << this->arr[i] << "\t" << this->arr[i + 1] << "\t" << this->arr[i + 2] << endl;
}

UniformCostSearch::UniformCostSearch(int pb[], Node*& par) {
	this->leftMost = this->left = this->right = this->rightMost = NULL;
	this->parent = par;
	this->g = this->f = par->getG() + 1;
	copy(pb, pb + 9, this->arr);
	this->calcH();
	this->nodeVal = 0;
	this->calcNodeVal();
}

UniformCostSearch::UniformCostSearch(int pb[]) {
	this->leftMost = this->left = this->right = this->rightMost = this->parent = NULL;
	this->nodeVal = this->g = this->h = this->f = 0;
	copy(pb, pb + 9, this->arr);
	this->calcNodeVal();
}

void UniformCostSearch::calcH() {
	this->h = 0;
}

MisplacedTile::MisplacedTile(int pb[], Node*& par) {
	this->leftMost = this->left = this->right = this->rightMost = NULL;
	this->parent = par;
	this->g = par->getG() + 1;
	this->h = 0;
	copy(pb, pb + 9, this->arr);
	this->calcH();
	this->f = this->g + this->h;
	this->nodeVal = 0;
	this->calcNodeVal();
}

MisplacedTile::MisplacedTile(int pb[]) {
	this->leftMost = this->left = this->right = this->rightMost = this->parent = NULL;
	this->g = this->h = 0;
	copy(pb, pb + 9, this->arr);
	this->calcH();
	this->f = this->h;
	this->nodeVal = 0;
	this->calcNodeVal();
}

void MisplacedTile::calcH() {
	int x, y, row;
	for (unsigned int i = 0; i < 9; ++i) {
		if (this->arr[i] != i + 1 && this->arr[i] != 0) {
			x = i; //index of where the misplaced number is at
			y = arr[i] - 1; //index of where it is supposed to be 
			if (x > y) // swaps the smallest value to x so that we only focus on moving down 
				swap(x, y);
			//Find which row the correct index is located in 
			if (y < 3)
				row = 0; //first row (indices: 0, 1, 2)
			else if (y < 6)
				row = 3; //second row (indices: 3, 4, 5)
			else
				row = 6; //third row (indices: 6, 7, 8)
			//move x down to the row where it's supposed to be
			while (x < row) {
				x += 3;
				this->h += 1;
			}
			if (x < y)
				swap(x, y); //largest value goes to x so we don't add negatives
			this->h += x - y;
		}
	}
}

EucledeanDistance::EucledeanDistance(int pb[], Node*& par) {
	this->leftMost = this->left = this->right = this->rightMost = NULL;
	this->parent = par;
	this->g = par->getG() + 1;
	copy(pb, pb + 9, this->arr);
	this->calcH();
	this->f = this->g + this->h;
	this->nodeVal = 0;
	this->calcNodeVal();
}

EucledeanDistance::EucledeanDistance(int pb[]) {
	this->leftMost = this->left = this->right = this->rightMost = this->parent = NULL;
	this->g = 0;
	copy(pb, pb + 9, this->arr);
	this->calcH();
	this->f = this->h;
	this->nodeVal = 0;
	this->calcNodeVal();
}

void EucledeanDistance::calcH() {
	//used to store the coordinates of the misplaced item and where it is supposed to be
	//(x1, y1) - coordinates of missplaced tile
	//(x2, y2) - coordinates of correct location
	int x1, y1, x2, y2; 
	for (unsigned int i = 0; i < 9; ++i) {
		if (this->arr[i] != i + 1 && this->arr[i] != 0) { //if item in array is not where it belongs
			this->findCoordinates(x1, y1, i);
			this->findCoordinates(x2, y2, this->arr[i] - 1);
			this->h += sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)); 
		}
	}
}

void EucledeanDistance::findCoordinates(int& x, int& y, int index) {
	if (index % 3 == 0)
		x = 0;
	else if (index == 1 || index == 4 || index == 7)
		x = 1;
	else
		x = 2;
	if (index < 3)
		y = 2;
	else if (index < 6)
		y = 1;
	else
		y = 0;
}

Tree::Tree(int arr[], int type) {
	this->algorithmType = type;
	if (this->algorithmType == 1)
		this->root = new UniformCostSearch(arr);
	else if (this->algorithmType == 2)
		this->root = new MisplacedTile(arr);
	else
		this->root = new EucledeanDistance(arr);
	this->expandedNodesCount = 0;
	this->maxFrontierSize = 0;
	this->insertCreatedNodes(this->root->nodeVal);
}

void Tree::buildTree() {
	//push root into frontier
	this->insertFrontier(this->root);
	this->nodeValues.clear();
	Node* curNode; //where we store the nodes from the frontier
	Node* goalStateMinPath = NULL; //pointer to the smallest f(n) value goal state
	bool foundGoalState = false; //bool we we'll use later to detemine if we have already found a goal state
	cout << "Expanding state" << endl;
	this->root->printPuzzleBoard();
	cout << endl;
	if (this->root->nodeVal == GOAL_VALUE) {
		this->maxFrontierSize = 1;
		this->printFinalSolution(this->root);
		return;
	}
	while (this->frontier.size() != 0) {
		if (this->maxFrontierSize < this->frontier.size())
			this->maxFrontierSize = this->frontier.size(); //updating max frontier size counter
		curNode = this->frontier.at(0);
		this->frontier.erase(this->frontier.begin());
		/*If we have already found a goal state, we want to compare the f(n) values
		between the two goal states so we can choose the cheapest path to a goal 
		state*/
		if (foundGoalState) {
			/*curNode will be pointing to the node with the smallest f(n) value
			in the frontier. If the f(n) value of the found goal state is cheaper than the cheapest 
			f(n) value in the frontier (at index 0, then we know for sure that is the cheapest
			f(n) score to find the goal state in the tree so we no longer need to traverse the tree. 
			If there is a cheaper f(n) score in the frontier, there could be a potential cheaper path
			so we continue the search*/
			if (goalStateMinPath->getF() <= curNode->getF()) {
				this->printFinalSolution(goalStateMinPath);
				return;
			}		
		}
		if (curNode->nodeVal == GOAL_VALUE) {
			goalStateMinPath = curNode;
			if (!foundGoalState) {
				
				foundGoalState = true;
			}
		}
		if(curNode->nodeVal != GOAL_VALUE)
			this->insertNodeValues(curNode->nodeVal);
		//expand node -> add node children
		this->expandNode(curNode);
	}
	cout << "No valid path" << endl;
}

void Tree::expandNode(Node* &curNode) {
	if (curNode == NULL || curNode->nodeVal == GOAL_VALUE)
		return;
	int newArray[9];
	//Find index of blank space
	int blank = 9;
	blank = curNode->findBlank();
	//Copy contents of parents array into child's new array
	copy(curNode->arr, curNode->arr + 9, newArray);
	//Determine all moves and create new nodes for them if they don't already exist (exception for goal states)
	if (blank > 2) { //make move up
		swap(newArray[blank], newArray[blank - 3]);
		this->addNode(newArray, curNode);
		copy(curNode->arr, curNode->arr + 9, newArray);
	}
	if (blank < 6) { //make move down
		swap(newArray[blank], newArray[blank + 3]);
		this->addNode(newArray, curNode);
		copy(curNode->arr, curNode->arr + 9, newArray);
	}
	if (blank % 3 != 0) { //make move left
		swap(newArray[blank], newArray[blank - 1]);
		this->addNode(newArray, curNode);
		copy(curNode->arr, curNode->arr + 9, newArray);
	}
	if (blank != 2 && blank != 5 && blank != 8) { //make move right
		swap(newArray[blank], newArray[blank + 1]);
		this->addNode(newArray, curNode);
		copy(curNode->arr, curNode->arr + 9, newArray);
	}
	//If the curNode's children are empty, then I won't be counting it as an expanded node
	//not sure if I have to change it to always increase the count
	//even if curNode is a deadend
	if (curNode->leftMost != NULL) 
		++this->expandedNodesCount;
}

void Tree::addNode(int arr[], Node*& parent) {
	Node* newNode;
	if (this->algorithmType == 1)
		newNode = new UniformCostSearch(arr, parent);
	else if (this->algorithmType == 2)
		newNode = new MisplacedTile(arr, parent);
	else
		newNode = new EucledeanDistance(arr, parent);
	if (!binary_search(this->nodeValues.begin(), this->nodeValues.end(), newNode->nodeVal) && !inFrontier(newNode->nodeVal)) {
		if (!binary_search(this->createdNodes.begin(), this->createdNodes.end(), newNode->nodeVal)) {
			parent->addChild(newNode);
			this->insertFrontier(newNode);
			if (newNode->nodeVal != GOAL_VALUE)
				this->insertCreatedNodes(newNode->nodeVal);
		}
		else {
			newNode->parent = NULL;
			delete newNode;
		}
	}
	else {
		//cout << "Node already exists" << endl;
		newNode->parent = NULL;
		delete newNode;
	}
}

//We insert new discovered nodes into the frontier in ascending order 
//First item in the vector is the smallest value f
//Essentially becoming a priority min queue
void Tree::insertFrontier(Node*& insertNode) {
	//if the frontier is empty, insert automatically
	if (this->frontier.size() == 0) { 
		this->frontier.push_back(insertNode);
		return;
	}
	//if insertNode's f is smaller than first Node's f in the vector, insert at the front
	if (insertNode->getF() < this->frontier.at(0)->getF()) { 
		this->frontier.insert(this->frontier.begin(), insertNode);
		return;
	}
	//if insertNode's f is larger than or equal to the last node's f, push_back
	if (insertNode->getF() >= this->frontier.at(frontier.size() - 1)->getF()) {
		this->frontier.push_back(insertNode);
		return;
	}
	//At this point, insertNode belongs somewhere in the middle of the vector
	//so we traverse to see where to insert it at 
	unsigned int i = 0;
	while (this->frontier.at(i)->getF() <= insertNode->getF()) {
		++i;
	}
	this->frontier.insert(this->frontier.begin() + i, insertNode);
}

void Tree::printFinalSolution(Node*& curNode) {
	if (curNode == NULL)
		return;
	printFinalSolution(curNode->parent);
	if (curNode->leftMost != NULL) {
		cout << "The best state to expand with g(n) = " << curNode->getG() << " and h(n) = " << curNode->getH() << " is..." << endl;
		curNode->printPuzzleBoard();
		cout << "Expanding this node" << endl << endl;
	}
	if (curNode->nodeVal == GOAL_VALUE) {
		cout << endl << "Goal!!!" << endl << endl;
		curNode->printPuzzleBoard();
		cout << endl;
		cout << "To solve this problem the search algorithm expanded a total of " << this->expandedNodesCount << " nodes." << endl;
		cout << "The maximum number of nodes in the queue at any time: " << this->maxFrontierSize << endl;
	}
}

bool Tree::inFrontier(int targetVal) {
	if (this->frontier.size() == 0)
		return false;
	for (unsigned int i = 0; i < this->frontier.size(); ++i) {
		if (this->frontier.at(i)->nodeVal == targetVal)
			return true;
	}
	return false;
}


void Tree::insertCreatedNodes(int targetVal) {
	//if the vector is empty, insert automatically
	if (this->createdNodes.size() == 0) {
		this->createdNodes.push_back(targetVal);
		return;
	}
	//if insertNode's f is smaller than first Node's f in the vector, insert at the front
	if (targetVal < this->createdNodes.at(0)) {
		this->createdNodes.insert(this->createdNodes.begin(), targetVal);
		return;
	}
	//if insertNode's f is larger than or equal to the last node's f, push_back
	if (targetVal >= this->createdNodes.at(this->createdNodes.size() - 1)) {
		this->createdNodes.push_back(targetVal);
		return;
	}
	//At this point, insertNode belongs somewhere in the middle of the vector
	//so we traverse to see where to insert it at 
	unsigned int i = 0;
	while (this->createdNodes.at(i) <= targetVal) {
		++i;
	}
	this->createdNodes.insert(this->createdNodes.begin() + i, targetVal);
}
void Tree::insertNodeValues(int targetVal) {
	//if the vector is empty, insert automatically
	if (this->nodeValues.size() == 0) {
		this->nodeValues.push_back(targetVal);
		return;
	}
	//if insertNode's f is smaller than first Node's f in the vector, insert at the front
	if (targetVal < this->nodeValues.at(0)) {
		this->nodeValues.insert(this->nodeValues.begin(), targetVal);
		return;
	}
	//if insertNode's f is larger than or equal to the last node's f, push_back
	if (targetVal >= this->nodeValues.at(this->nodeValues.size() - 1)) {
		this->nodeValues.push_back(targetVal);
		return;
	}
	//At this point, insertNode belongs somewhere in the middle of the vector
	//so we traverse to see where to insert it at 
	unsigned int i = 0;
	while (this->nodeValues.at(i) <= targetVal) {
		++i;
	}
	this->nodeValues.insert(this->nodeValues.begin() + i, targetVal);
}
