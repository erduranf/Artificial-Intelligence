#pragma once
//external variables declared in main.cpp
extern unsigned int nodesExpanded;
extern unsigned int maxFringeSize;
extern unsigned int solutionLength;
extern double runtime;

using namespace std;

void printInstructions(void) {
	cout << "Welcome to my 8-Puzzle Solver! You may change the initial board setting in \"main.cpp\". "
		<< "Please choose which search algorithm you wish to use: \n"
		"1. Breadth-First\n"
		"2. Depth-Limited\n"
		"3. Iterative Deepening\n\n"
		"Your choice: ";
}

//displays contents of the board
void printBoard(char const board[][3]) { 
	char row, col;

	for (row = 0; row < 3; ++row) {
		cout << "+---+---+---+" << endl;
		for (col = 0; col < 3; ++col) {
			cout << "| " << board[row][col] << " ";
		}
		cout << "|" << endl;
	}
	cout << "+---+---+---+" << endl << endl;
}

//takes solution path as input and displays moves
void printSolution(struct sol_Path *path) {
	//check if solution exists
	if (!path) {
		cout << "No solution found.\n" << endl;
		return;
	}

	//if the initial state is already the goal state
	if (!path->next) {
		cout << "No moves needed. The initial state is already the goal state.\n";
		return;
	}

	cout << "SOLUTION FOUND: (Actions are listed with respect to blank (0))\n";
	
	char *move[4] = { "LEFT", "RIGHT", "UP", "DOWN" }; //what to display
	int counter = 1;

	for (path = path->next; path; path = path->next, counter++) //skip initial state since it is NO_OP
		cout << counter << ". " << move[path->action] << ", ";
	cout << "Goal!" << endl << endl;
	cout << "DETAILS:" << endl <<
		" - Solution length		: " << solutionLength << endl <<
		" - Nodes expanded		: " << nodesExpanded << endl <<
		" - Maximum size of the fringe	: " << maxFringeSize << endl <<
		" - Runtime			: " << runtime << " s" << endl <<
		" - Memory used			: " << maxFringeSize * sizeof(Node) << " bytes" << endl;
}
