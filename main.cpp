#include <iostream>
#include <string>
#include <stdlib.h>
#include "tree.h"
using namespace std;

void defaultPuzzle(int pb[], int size);
void userCreatedPuzzle(int pb[], int size);
bool inPuzzleBoard(int pb[], int size, int target);
void printPuzzleBoard(int pb[]);

int main() {
	int input = 0;
	const int puzzleSize = 9;
	int puzzleBoard[puzzleSize];
	cout << "Welcome to 862009642 8 puzzle solver." << endl;
	while (input != 1 && input != 2) {
		cout << "Type \"1\" to use a default puzzle or \"2\" to enter your own puzzle." << endl;
		cin >> input;
		cout << endl;
		if (input == 1) {
			defaultPuzzle(puzzleBoard, puzzleSize);
		}
		else if (input == 2)
			userCreatedPuzzle(puzzleBoard, puzzleSize);
		else
			cout << "Invalid Input." << endl;
	}
	cout << endl;
	input = 0;
	while (input < 1 || input > 3) {
		cout << "Enter your choice of algorithm " << endl
			<< "1. Uniform Cost Search" << endl
			<< "2. A* with the Misplaced Tile heuristic" << endl
			<< "3. A* with Euclidian distance heuristic" << endl;
		cin >> input;
		if (input < 1 || input > 3)
			cout << "Invalid Input." << endl;
	}
	Tree* searchTree = new Tree(puzzleBoard, input);
	searchTree->buildTree();
	return 0;
}

void defaultPuzzle(int pb[], int size) {
	for (int i = 0; i < size; ++i) {
		if (i == 4)
			pb[i] = 8;
		else if (i == 5)
			pb[i] = 0;
		else if (i == 6)
			pb[i] = 7;
		else if (i == 7)
			pb[i] = 6;
		else if (i == 8)
			pb[i] = 5;
		else 
			pb[i] = i + 1;
	}
}

void userCreatedPuzzle(int pb[], int size) {
	int index = 0, x; //index keeps track of location in arr, x is temp var where the input num would be stored
	string input; //user input where the target nums are stored in (in a string so have to convert to int)
	cin.ignore(); //clear buffer before getline
	cout << "Enter your puzzle, use a zero to represent the blank" << endl;
	for (int i = 0; i < 3; ++i) {
		cout << "Enter the ";
		if (i == 0)
			cout << "first";
		else if (i == 1)
			cout << "second";
		else
			cout << "third";
		cout << " row, use space or tabs between numbers\t";
		input = "";
		getline(cin, input);
		if (input.size() == 5) { //input per line should only be 5 characters long including space/tab
			for (unsigned int j = 0; j < input.size(); j += 2) { //loop to go through the row and extract ints
				if (isdigit(input[j])) { //checks if the char at the index is an int
					x = input[j] - '0'; //convert char to int to store in int arr
					if (x == 9) //at this point, 9 would be the only invalid input num
						goto reset;
					if (inPuzzleBoard(pb, index, x)) {//if the num already exists in the board, then invalid
						reset:				//if we encounter an invalid number while inputting the numbers, 
						if (index < 3)		//we have to reset the index back to the beginning of the row to 
							index = 0;		//overwrite the ints we already put in 
						else if (index < 6)
							index = 3;
						else
							index = 6;
						goto invalid;
					}
					//cout << "Inserting " << x << " at index " << index << endl;
					pb[index] = x; //if all the checks pass then we will input the number into the puzzle board
					++index; //add index by one to keep track of where we are in the arr
				}
				else
					goto invalid;
			}
		}
		else {
			invalid:
			cout << "Invalid Input" << endl;
			--i; //subtract i by one so that it would ++i immedietly after to reset and input the same row 
		}
	}
}

bool inPuzzleBoard(int pb[], int size, int target) { //checks if target num already exists in the array
	if (size == 0)
		return false;
	for (int i = 0; i < size; ++i) {
		if (pb[i] == target)
			return true;
	}
	return false;
}

void printPuzzleBoard(int pb[]) {
	for (unsigned int i = 0; i < 7; i += 3)
		cout << pb[i] << "\t" << pb[i + 1] << "\t" << pb[i + 2] << endl;
}
