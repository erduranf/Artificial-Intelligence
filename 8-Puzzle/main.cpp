/* Ferhat Erduran
   1600001092
   CSE0440 - Assignment #1
*/

#include <iostream>
#include <time.h>
#include "state.h"
#include "list.h"
#include "node.h"
#include "io.h"

using namespace std;

unsigned int nodesExpanded = 0; //number of expanded nodes
unsigned int maxFringeSize = 0; //maximum size of the fringe
unsigned int solutionLength = 0; //number of moves in solution
double runtime; //elapsed time (in milliseconds)

sol_Path* BF_search(State *, State *); //breadth-first search
sol_Path* DL_Search(State *, State *, unsigned int); //depth-limited search
sol_Path* ID_Search(State *, State *, unsigned int, unsigned int); //iterative deepening search

int main() {
	unsigned int choice, limit;
	State initial, goal;
	sol_Path *bfs, *dls, *ids;

	//initial board state can be changed here
	initial.board[0][0] = '7';
	initial.board[0][1] = '2';
	initial.board[0][2] = '4';
	initial.board[1][0] = '5';
	initial.board[1][1] = '0';
	initial.board[1][2] = '6';
	initial.board[2][0] = '8';
	initial.board[2][1] = '3';
	initial.board[2][2] = '1';
	initial.action = NO_OP;

	//goal board state can be changed here
	goal.board[0][0] = '0';
	goal.board[0][1] = '1';
	goal.board[0][2] = '2';
	goal.board[1][0] = '3';
	goal.board[1][1] = '4';
	goal.board[1][2] = '5';
	goal.board[2][0] = '6';
	goal.board[2][1] = '7';
	goal.board[2][2] = '8';
	goal.action = NO_OP;

	cout << "INITIAL BOARD STATE:" << endl;
	printBoard(initial.board);

	cout << "GOAL BOARD STATE:" << endl;
	printBoard(goal.board);

	printInstructions();
	cin >> choice;

	if (choice == 1) {
		cout << "\nPerforming Breadth-First Search: \n" << endl;
		bfs = BF_search(&initial, &goal);
		if (bfs) printSolution(bfs);
		else cout << "Search failed." << endl;
		destroySolution(&bfs);
	}
	else if (choice == 2) {
		cout << "\nPlease enter limit: ";
		cin >> limit;
		cout << "\nPerforming Depth-Limited Search: \n" << endl; 
		dls = DL_Search(&initial, &goal, limit);
		if (dls) printSolution(dls);
		else cout << "Search failed. Please try a higher limit."<< endl;
		destroySolution(&dls);
	}
	else if (choice == 3) {
		unsigned int factor;
		cout << "\nPlease enter initial limit: ";
		cin >> limit;
		cout << "Please enter limit-deepening factor: ";
		cin >> factor;
		cout << "\nPerforming Iterative-Deepening Search: \n" << endl;
		ids = ID_Search(&initial, &goal, limit, factor);
		if (ids) printSolution(ids);
		else cout << "Search failed." << endl;
		destroySolution(&ids);
	}
	else cout << "You have entered an invalid input. Please try again." << endl;

	cin.ignore(1, 'n');
	cin.get();
}

//Implementation of Breadth First Search algorithm
sol_Path* BF_search(State *initial, State *goal) {
	NodeList *fringe = nullptr;
	NodeList *children = nullptr;
	Node *node = nullptr;

	clock_t start = clock();//start timer

	//initialize the fringe with the root node of the search tree
	push(createNode(0, initial, nullptr), &fringe);
	Node *root = fringe->head->cur; //for deallocating the generated tree

	//while there is a node in the fringe to expand
	while (fringe->nodeCount > 0) {
		//pop the last node (tail) of the fringe, FIFO
		node = BFS_pop(&fringe);

		//if the state of the node is the goal state
		if (isGoal(node->state, goal))
			break;

		//else expand the node and update the expanded-nodes counter
		children = expandNode(node);
		nodesExpanded++;

		//add the node's children to the fringe
		pushList(&children, fringe);
	}

	//determine the time elapsed
	runtime = (double)(clock() - start) / CLOCKS_PER_SEC;

	//get solution path in order from the root, if it exists
	sol_Path *pathHead = nullptr;
	sol_Path *newPathNode = nullptr;

	while (node) {
		newPathNode = new sol_Path;
		newPathNode->action = node->state->action;
		newPathNode->next = pathHead;
		pathHead = newPathNode;

		//update the solution length and move on the next node
		solutionLength++;
		node = node->parent;
	}
	solutionLength--; //uncount the root node
	destroyTree(root); //deallocate the generated tree
	return pathHead;
}

//Implementation of Depth-Limited Search algorithm
sol_Path* DL_Search(State *initial, State *goal,  unsigned int limit) {
	NodeList *fringe = nullptr;
	NodeList *children = nullptr;
	Node *node = nullptr;

	clock_t start = clock(); //start timer

	//initialize the fringe with the root node of the search tree
	push(createNode(0, initial, nullptr), &fringe);
	Node *root = fringe->head->cur; //for deallocating the generated tree

	//while there is a node in the fringe to expand
	while (fringe->nodeCount > 0) {
		//pop the first node (head) of the fringe, LIFO
		node = DFS_pop(&fringe);

		//if the state of the node is the goal state
		if (isGoal(node->state, goal))
			break;

		if (node->depth < limit) {
			//else expand the node and update the expanded-nodes counter
			children = expandNode(node);
			nodesExpanded++;

			//add the node's children to the fringe
			pushList(&children, fringe);
		}
	}

	//get solution path in order from the root, if it exists
	sol_Path *pathHead = nullptr;
	sol_Path *newPathNode = nullptr;
	
	if (isGoal(node->state, goal)) {
		//determine the time elapsed
		runtime = (double)(clock() - start) / CLOCKS_PER_SEC;

		while (node) {
			newPathNode = new sol_Path;
			newPathNode->action = node->state->action;
			newPathNode->next = pathHead;
			pathHead = newPathNode;

			//update the solution length and move on the next node
			solutionLength++;
			node = node->parent;
		}
	}
	else { //if no solution is found within limits, return NULL
		destroyTree(root);
		return nullptr;
	}
	solutionLength--; //uncount root 
	destroyTree(root);
	return pathHead;
}

//Implementation of Iterative Deepening Search algorithm
sol_Path* ID_Search(State *initial, State *goal, unsigned int limit, unsigned int factor) {
	NodeList *fringe = nullptr;
	NodeList *children = nullptr;
	Node *node = nullptr;

	clock_t start = clock(); //start timer

	//initialize the fringe with the root node of the search tree
	push(createNode(0, initial, nullptr), &fringe);
	Node *root = fringe->head->cur; //for deallocating the generated tree
	
	while (true)
	{
		//while there is a node in the fringe to expand
		while (fringe->nodeCount > 0) {
			//pop the first node (head) of the fringe, LIFO
			node = DFS_pop(&fringe);
			//if the state of the node is the goal state
			if (isGoal(node->state, goal))
				break;

			if (node->depth < limit) {
				//else expand the node and update the expanded-nodes counter
				children = expandNode(node);
				nodesExpanded++;

				//add the node's children to the fringe
				pushList(&children, fringe);
			}
		}
		if (isGoal(node->state, goal))
			break;
		cout << "The algorithm couldn't find a solution with limit " << limit <<
			", new limit is " << limit << " + " << factor << " = " << limit + factor << "." << endl;
		limit += factor; //increase the limit by user-defined factor
		fringe = nullptr, children = nullptr, node = nullptr;
		push(createNode(0, initial, nullptr), &fringe);
	}

	//get solution path in order from the root, if it exists
	sol_Path *pathHead = nullptr;
	sol_Path *newPathNode = nullptr;
	if (isGoal(node->state, goal)) {
		//determine the time elapsed
		runtime = (double)(clock() - start) / CLOCKS_PER_SEC;

		while (node) {
			newPathNode = new sol_Path;
			newPathNode->action = node->state->action;
			newPathNode->next = pathHead;
			pathHead = newPathNode;

			//update the solution length and move on the next node
			solutionLength++;
			node = node->parent;
		}
	}
	else {
		destroyTree(root);
		return nullptr;
	}
	solutionLength--; //uncount root 
	destroyTree(root);
	return pathHead;
}
