#pragma once
#define BLANK '0'

//maps numbers to the moves for better readability
typedef enum Move { LEFT, RIGHT, UP, DOWN, NO_OP } Move;

typedef struct State {
	Move action; //action that resulted in this state
	char board[3][3]; //3x3 array to store game state
} State;

//creates a new state if move m is valid; returns a pointer to the new state on success, NULL on failure
State* createState(State *s, Move m) {
	State *newState = new State;
	char row, col, temp;

	//copy the board configuration and learn the position of BLANK
	for (char i = 0; i < 3; i++) {
		for (char j = 0; j < 3; j++) {
			if (s->board[i][j] == BLANK)
				row = i, col = j;
			newState->board[i][j] = s->board[i][j];
		}
	}

	//test if move is valid based on current state; if yes, swap the relevant board values
	if (m == LEFT && col - 1 >= 0) {
		temp = newState->board[row][col - 1];
		newState->board[row][col - 1] = BLANK;
		newState->board[row][col] = temp;
		newState->action = LEFT;
		return newState;
	}
	else if (m == RIGHT && col + 1 <= 2) {
		temp = newState->board[row][col + 1];
		newState->board[row][col + 1] = BLANK;
		newState->board[row][col] = temp;
		newState->action = RIGHT;
		return newState;
	}
	else if (m == UP && row - 1 >= 0) {
		temp = newState->board[row - 1][col];
		newState->board[row - 1][col] = BLANK;
		newState->board[row][col] = temp;
		newState->action = UP;
		return newState;
	}
	else if (m == DOWN && row + 1 <= 2) {
		temp = newState->board[row  + 1][col];
		newState->board[row + 1][col] = BLANK;
		newState->board[row][col] = temp;
		newState->action = DOWN;
		return newState;
	}

	delete newState;
	return NULL;
}

//tests if the test state matches the goal state; returns true on success, false on failure
bool isGoal(State const *test, State const *goal) {
	int row = 3, col;
	while (row--) {
		col = 3;
		while (col--) {
			if (test->board[row][col] != goal->board[row][col]) return false;
		}
	}
	return true;
}