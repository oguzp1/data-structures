#include "maze.h"
#include <iostream>
#include <cstdio>
#include <ctime>

using namespace std;

void initializeMaze(cell m[][MAZESIZE]);
void resetVisited(cell m[][MAZESIZE]);
void printMaze(cell m[][MAZESIZE]);

bool noneUnvisited(cell m[][MAZESIZE]);
bool cellHasNeighbours(cell m[][MAZESIZE], int cur_i, int cur_j);
void generateMaze(cell m[][MAZESIZE], CellStack *cellStack, int ipos, int jpos);

int totalWays(cell m[][MAZESIZE], int cur_i, int cur_j);
void pathfind(cell m[][MAZESIZE], CellStack *nodes, CellStack *coords, int fin_i, int fin_j, int ipos, int jpos);

int main() {
	cell m[MAZESIZE][MAZESIZE];
	CellStack cellStack, nodes, coords;
	int i_i, j_i, i_f, j_f;
	char c;

	cellStack.initialize();
	nodes.initialize();
	coords.initialize();
	srand(time(NULL));

	do {
		initializeMaze(m);

		cout << "Please enter the initial points: ";
		cin >> i_i;
		cin >> j_i;
		getchar();

		generateMaze(m, &cellStack, i_i, j_i);
		printMaze(m);

		resetVisited(m);

		cout << "Please enter the final points: ";
		cin >> i_f;
		cin >> j_f;
		getchar();

		pathfind(m, &nodes, &coords, i_f, j_f, i_i, j_i);

		cout << "Enter \'e\' character to exit: ";
		c = getchar();
		getchar();

		cellStack.clearStack();
		nodes.clearStack();
		coords.clearStack();
	} while (c != 'e');

	return 0;
}

void initializeMaze(cell m[][MAZESIZE]) {
	for (int i = 0; i < MAZESIZE; ++i) {
		for (int j = 0; j < MAZESIZE; ++j) {
			m[i][j].up = m[i][j].down = m[i][j].left = m[i][j].right = true;
			m[i][j].visited = false;
			m[i][j].val = ' ';
		}
	}
}

void resetVisited(cell m[][MAZESIZE]) {
	for (int i = 0; i < MAZESIZE; ++i) {
		for (int j = 0; j < MAZESIZE; ++j)
			m[i][j].visited = false;
	}
}

void printMaze(cell m[][MAZESIZE]) {
	for (int i = 0; i < MAZESIZE; ++i) {
		for (int j = 0; j < MAZESIZE; ++j) {
			if (m[i][j].up)
				cout << " -";
			else
				cout << "  ";
		}
		cout << endl;
		cout << "|";

		for (int j = 0; j < MAZESIZE; ++j) {
			if (m[i][j].right)
				cout << m[i][j].val << "|";
			else
				cout << m[i][j].val << " ";
		}
		cout << endl;
	}

	for (int i = 0; i < MAZESIZE; ++i) {
		if (m[MAZESIZE - 1][i].down)
			cout << " -";
	}
	cout << endl;
}

bool noneUnvisited(cell m[][MAZESIZE]) {
	for (int i = 0; i < MAZESIZE; ++i) {
		for (int j = 0; j < MAZESIZE; ++j) {
			if (!m[i][j].visited)
				return false;
		}
	}
	return true;
}

bool cellHasNeighbours(cell m[][MAZESIZE], int cur_i, int cur_j) {
	if (cur_i > 0 && !m[cur_i - 1][cur_j].visited)
		return true;
	if (cur_i < MAZESIZE - 1 && !m[cur_i + 1][cur_j].visited)
		return true;
	if (cur_j > 0 && !m[cur_i][cur_j - 1].visited)
		return true;
	if (cur_j < MAZESIZE - 1 && !m[cur_i][cur_j + 1].visited)
		return true;
	return false;
}

void generateMaze(cell m[][MAZESIZE], CellStack *cellStack, int ipos, int jpos) {
	m[ipos][jpos].visited = true;

	if (noneUnvisited(m))
		return;

	if (cellHasNeighbours(m, ipos, jpos)) {
		int randvalues[4], maxval, direction;

		for (int i = 0; i < 4; ++i)
			randvalues[i] = rand();

		direction = 0;
		maxval = -1;

		if (ipos > 0 && !m[ipos - 1][jpos].visited && randvalues[UP - 1] > maxval) {
			maxval = randvalues[UP - 1];
			direction = UP;
		}
		if (ipos < MAZESIZE - 1 && !m[ipos + 1][jpos].visited && randvalues[DOWN - 1] > maxval) {
			maxval = randvalues[DOWN - 1];
			direction = DOWN;
		}
		if (jpos > 0 && !m[ipos][jpos - 1].visited && randvalues[LEFT - 1] > maxval) {
			maxval = randvalues[LEFT - 1];
			direction = LEFT;
		}
		if (jpos < MAZESIZE - 1 && !m[ipos][jpos + 1].visited && randvalues[RIGHT - 1] > maxval) {
			direction = RIGHT;
		}

		cellStack->push(ipos, jpos);

		switch (direction) {
		case UP:
			m[ipos][jpos].up = false;
			m[ipos - 1][jpos].down = false;
			generateMaze(m, cellStack, ipos - 1, jpos);
			break;
		case DOWN:
			m[ipos][jpos].down = false;
			m[ipos + 1][jpos].up = false;
			generateMaze(m, cellStack, ipos + 1, jpos);
			break;
		case LEFT:
			m[ipos][jpos].left = false;
			m[ipos][jpos - 1].right = false;
			generateMaze(m, cellStack, ipos, jpos - 1);
			break;
		case RIGHT:
			m[ipos][jpos].right = false;
			m[ipos][jpos + 1].left = false;
			generateMaze(m, cellStack, ipos, jpos + 1);
			break;
		}
	}
	else if (!cellStack->isEmpty()) {
		int popped_ipos, popped_jpos;
		cellStack->pop(&popped_ipos, &popped_jpos);
		generateMaze(m, cellStack, popped_ipos, popped_jpos);
	}
	else {
		int randval, maxval = -1, max_i, max_j;

		for (int i = 0; i < MAZESIZE; ++i) {
			for (int j = 0; j < MAZESIZE; ++j) {
				randval = rand();
				if (!m[i][j].visited && randval > maxval) {
					maxval = randval;
					max_i = i;
					max_j = j;
				}
			}
		}

		generateMaze(m, cellStack, max_i, max_j);
	}
}

int totalWays(cell m[][MAZESIZE], int cur_i, int cur_j) {
	int counter = 0;
	if (cur_i > 0 && !m[cur_i][cur_j].up && !m[cur_i - 1][cur_j].visited)
		++counter;
	if (cur_i < MAZESIZE - 1 && !m[cur_i][cur_j].down && !m[cur_i + 1][cur_j].visited)
		++counter;
	if (cur_j > 0 && !m[cur_i][cur_j].left && !m[cur_i][cur_j - 1].visited)
		++counter;
	if (cur_j < MAZESIZE - 1 && !m[cur_i][cur_j].right && !m[cur_i][cur_j + 1].visited)
		++counter;
	return counter;
}

void pathfind(cell m[][MAZESIZE], CellStack *nodes, CellStack *coords, int fin_i, int fin_j, int ipos, int jpos) {
	m[ipos][jpos].val = 'o';
	m[ipos][jpos].visited = true;
	coords->push(ipos, jpos);

	if (ipos == fin_i && jpos == fin_j) {
		printMaze(m);
		return;
	}

	int totalways = totalWays(m, ipos, jpos);

	if (totalways) {
		int randvalues[4], maxval, direction;

		for (int i = 0; i < 4; ++i)
			randvalues[i] = rand();

		direction = 0;
		maxval = -1;

		if (totalways > 1)
			nodes->push(ipos, jpos);

		if (ipos > 0 && !m[ipos][jpos].up && !m[ipos - 1][jpos].visited && randvalues[UP - 1] > maxval) {
			maxval = randvalues[UP - 1];
			direction = UP;
		}
		if (ipos < MAZESIZE - 1 && !m[ipos][jpos].down && !m[ipos + 1][jpos].visited && randvalues[DOWN - 1] > maxval) {
			maxval = randvalues[DOWN - 1];
			direction = DOWN;
		}
		if (jpos > 0 && !m[ipos][jpos].left && !m[ipos][jpos - 1].visited && randvalues[LEFT - 1] > maxval) {
			maxval = randvalues[LEFT - 1];
			direction = LEFT;
		}
		if (jpos < MAZESIZE - 1 && !m[ipos][jpos].right && !m[ipos][jpos + 1].visited && randvalues[RIGHT - 1] > maxval) {
			direction = RIGHT;
		}

		switch (direction) {
		case UP:
			pathfind(m, nodes, coords, fin_i, fin_j, ipos - 1, jpos);
			break;
		case DOWN:
			pathfind(m, nodes, coords, fin_i, fin_j, ipos + 1, jpos);
			break;
		case LEFT:
			pathfind(m, nodes, coords, fin_i, fin_j, ipos, jpos - 1);
			break;
		case RIGHT:
			pathfind(m, nodes, coords, fin_i, fin_j, ipos, jpos + 1);
			break;
		}
	}
	else {
		int i_t, j_t;

		do {
			coords->pop(&i_t, &j_t);
			m[i_t][j_t].val = ' ';
		} while (i_t != nodes->i_stack[nodes->cursor - 1] || j_t != nodes->j_stack[nodes->cursor - 1]);

		nodes->pop(&i_t, &j_t);

		pathfind(m, nodes, coords, fin_i, fin_j, i_t, j_t);
	}
}