#include <iostream>
#include <cstdlib>
#include <time.h>
#include "labyrinth.h"

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

using namespace std;

bool inStack(TravelStack *a, Coordinate *c) {
	for (int x = 0; x < a->cursor; ++x) {
		if (coordsEqual(a->stack + x, c))
			return true;
	}
	return false;
}

void printMap(char Maze[ROWCOUNT][COLCOUNT + 1]) {
	for (int a = 0; a < ROWCOUNT; ++a) {
		cout << Maze[a] << endl;
	}

	cout << endl << "Please enter a character to continue: ";
	getchar();
	getchar();
}

int decideRandom(LocationData *ld, int totalways) {
	int randomnum = rand() % totalways, 
		choice = UP, 
		available[4] = {0};
	
	if (ld->upAvailable)
		available[0] = 1;
	if (ld->downAvailable)
		available[1] = 1;
	if (ld->leftAvailable)
		available[2] = 1;
	if (ld->rightAvailable)
		available[3] = 1;

	while (!available[choice - 1])
		++choice;

	while (randomnum) {
		++choice;

		while (!available[choice - 1])
			++choice;

		--randomnum;
	}

	return choice;
}

int main() {
	char Maze[ROWCOUNT][COLCOUNT + 1] = { 
		"#O###################",
		"#     #      #    # #",
		"# ### # ###### ## # #",
		"# # #        # #  # #",
		"# # ### ######## ## #",
		"#     # # ##   #    #",
		"### # # # ## # # ####",
		"# # # # #    #      #",
		"# # # # # ######### #",
		"# # # # # #       # #",
		"# # #   # # #####   #",
		"# # ##### # #   #####",
		"#         #   #     #",
		"###################E#" 
	};
	Coordinate initialPoint = { 0, 1 },
			   finalPoint = { 13, 19 },
			   next;

	LocationData currentData = { initialPoint, false, true, false, false };

	TravelStack travelStack;
	NodeStack nodeStack;

	int cameFrom = 0, totalways;
	bool check = true;

	travelStack.initialize();
	nodeStack.initialize();

	travelStack.push(&initialPoint);
	printMap(Maze);
	srand(time(NULL));

	while (!coordsEqual(&currentData.coords, &finalPoint)) {
		if (check) {
			if (currentData.coords.i != 0 && Maze[currentData.coords.i - 1][currentData.coords.j] != '#' && Maze[currentData.coords.i - 1][currentData.coords.j] != 'O')
				currentData.upAvailable = true;
			if (currentData.coords.i != ROWCOUNT - 1 && Maze[currentData.coords.i + 1][currentData.coords.j] != '#' && Maze[currentData.coords.i - 1][currentData.coords.j] != 'O')
				currentData.downAvailable = true;
			if (currentData.coords.j != 0 && Maze[currentData.coords.i][currentData.coords.j - 1] != '#' && Maze[currentData.coords.i - 1][currentData.coords.j] != 'O')
				currentData.leftAvailable = true;
			if (currentData.coords.j != COLCOUNT - 1 && Maze[currentData.coords.i][currentData.coords.j + 1] != '#' && Maze[currentData.coords.i - 1][currentData.coords.j] != 'O')
				currentData.rightAvailable = true;

			switch (cameFrom) {
				case UP:
					currentData.upAvailable = false;
					break;
				case DOWN:
					currentData.downAvailable = false;
					break;
				case LEFT:
					currentData.leftAvailable = false;
					break;
				case RIGHT:
					currentData.rightAvailable = false;
					break;
			}
		}
		check = true;

		totalways = currentData.totalWays();

		if (totalways) {
			if (totalways > 1) {
				nodeStack.push(&currentData);

				int choice = decideRandom(&nodeStack.stack[nodeStack.cursor - 1], totalways);

				if (choice == UP) {
					nodeStack.stack[nodeStack.cursor - 1].upAvailable = false;
					next = { currentData.coords.i - 1, currentData.coords.j };
					cameFrom = DOWN;
				}
				else if (choice == DOWN) {
					nodeStack.stack[nodeStack.cursor - 1].downAvailable = false;
					next = { currentData.coords.i + 1, currentData.coords.j };
					cameFrom = UP;
				}
				else if (choice == LEFT) {
					nodeStack.stack[nodeStack.cursor - 1].leftAvailable = false;
					next = { currentData.coords.i, currentData.coords.j - 1 };
					cameFrom = RIGHT;
				}
				else {
					nodeStack.stack[nodeStack.cursor - 1].rightAvailable = false;
					next = { currentData.coords.i, currentData.coords.j + 1 };
					cameFrom = LEFT;
				}
			}
			else {
				if (currentData.upAvailable) {
					next = { currentData.coords.i - 1, currentData.coords.j };
					cameFrom = DOWN;
				}
				else if (currentData.downAvailable) {
					next = { currentData.coords.i + 1, currentData.coords.j };
					cameFrom = UP;
				}
				else if (currentData.leftAvailable) {
					next = { currentData.coords.i, currentData.coords.j - 1 };
					cameFrom = RIGHT;
				}
				else {
					next = { currentData.coords.i, currentData.coords.j + 1 };
					cameFrom = LEFT;
				}
			}

			Maze[next.i][next.j] = 'O';
			travelStack.push(&next);
			printMap(Maze);
			currentData = { next, false, false, false, false };
		}
		else {
			while (!coordsEqual(&travelStack.stack[travelStack.cursor - 1], &nodeStack.stack[nodeStack.cursor - 1].coords)) {
				Coordinate *temp = travelStack.pop();
				Maze[temp->i][temp->j] = ' ';
				printMap(Maze);
			}

			currentData = nodeStack.stack[nodeStack.cursor - 1];
			if (currentData.totalWays() == 1)
				nodeStack.pop();

			check = false;
		}
	}

	cout << "Points:" << endl;
	for (int a = 0; a < travelStack.cursor; ++a)
		cout << travelStack.stack[a].i << ", " << travelStack.stack[a].j << endl;
	getchar();
	return 0;
}
