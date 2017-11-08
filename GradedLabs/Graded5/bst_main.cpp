#include "bst.h"
#include <iostream>
#include <cstring>

using namespace std;

bool decision(Tree *t, char c) {
	treenode temp, *toDelete;
	char *tempstr, *tempstr2;
	int orderno = 1;

	switch (c) {
	case 'A':
	case 'a':
		cout << "Please enter a name:" << endl;
		temp.name = new char[NAME_LENGTH];
		cin.getline(temp.name, NAME_LENGTH);
		cout << "Please enter a number to add:" << endl;
		tempstr = new char[NUM_LENGTH];
		cin.getline(tempstr, NAME_LENGTH);
		temp.phonenum.initialize();
		temp.phonenum.add(tempstr);
		t->add(&temp);
		temp.phonenum.clear();
		delete[] temp.name;
		delete[] tempstr;
		break;
	case 'P':
	case 'p':
		t->printTree(t->root, &orderno);
		break;
	case 'V':
	case 'v':
		t->printTreeRec(t->root, 0);
		break;
	case 'S':
	case 's':
		cout << "Record name to be searched:" << endl;
		tempstr = new char[NAME_LENGTH];
		cin.getline(tempstr, NAME_LENGTH);
		if (!t->searchRecords(t->root, tempstr))
			cout << "No such record found." << endl;
		delete[] tempstr;
		break;
	case 'U':
	case 'u':
		t->printTree(t->root, &orderno);
		cout << "Record name to be updated:" << endl;
		tempstr = new char[NAME_LENGTH];
		cin.getline(tempstr, NAME_LENGTH);
		cout << "The new name of the record:" << endl;
		tempstr2 = new char[NAME_LENGTH];
		cin.getline(tempstr2, NAME_LENGTH);
		t->update(tempstr, tempstr2);
		delete[] tempstr;
		delete[] tempstr2;
		break;
	case 'D':
	case 'd':
		t->printTree(t->root, &orderno);
		cout << "Record name to be deleted:" << endl;
		tempstr = new char[NAME_LENGTH];
		cin.getline(tempstr, NAME_LENGTH);
		toDelete = t->deleteNode(tempstr);
		delete[] toDelete->name;
		toDelete->phonenum.clear();
		delete toDelete;
		delete[] tempstr;
		break;
	case 'E':
	case 'e':
		return false;
	}
	return true;
}

int main() {
	Tree t;
	char c;

	t.initialize();

	do {
		cout << "Binary Search Tree Phonebook..." << endl;
		cout << "A: Add record" << endl;
		cout << "P: Print all records" << endl;
		cout << "V: View tree structure" << endl;
		cout << "S: Search for record" << endl;
		cout << "U: Update record" << endl;
		cout << "D: Delete a record" << endl;
		cout << "E: Exit" << endl;
		cout << "-------------------------------" << endl;
		cout << "Your choice: ";
		cin >> c;
		getchar();
	} while (decision(&t, c));

	t.clear(t.root);
	return 0;
}