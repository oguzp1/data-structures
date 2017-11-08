#include "binarytree.h"
#include <iostream>
#include <cstdio>

using namespace std;

void search(Tree *t) {
	int choice, noSearched, stepnoInorder;

	cout << endl;
	cout << "1: Inorder Search" << endl;
	cout << "2: Breadth First Search" << endl;
	cout << "3: Depth First Search" << endl;
	do {
		cout << "Please choose search method: ";
		cin >> choice;
		getchar();
	} while (choice < 1 || choice > 3);

	t->printTree();
	do {
		cout << "Please enter the number to search for: ";
		cin >> noSearched;
		getchar();
	} while (noSearched < 0);

	switch (choice) {
		case 1:
			stepnoInorder = 0;
			if (t->InorderSearch(noSearched, t->root, &stepnoInorder))
				cout << "Ended at step no.: " << stepnoInorder << endl;
			else {
				cout << " -> X" << endl;
				cout << "Could not be found." << endl;
			}
			break;
		case 2:
			if (!t->BreadthFirstSearch(noSearched)) {
				cout << "X" << endl;
				cout << "Could not be found." << endl;
			}
			break;
		case 3:
			if (!t->DepthFirstSearch(noSearched)) {
				cout << "X" << endl;
				cout << "Could not be found." << endl;
			}
			break;
	}
}

int main() {
	Tree t;
	char c;

	t.initialize();
	
	do {
		search(&t);
		c = getchar();
	} while (c != 'e' && c != 'E');
	
	t.close();
	
	return 0;
}