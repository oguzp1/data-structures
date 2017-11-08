#include "bst.h"
#include <iostream>
#include <cstring>

using namespace std;

void NumberList::initialize() {
	head = NULL;
}

void NumberList::add(char *newNumber) {
	numbernode *newNumNode = new numbernode;
	newNumNode->number = new char[strlen(newNumber) + 1];
	strcpy(newNumNode->number, newNumber);
	newNumNode->next = NULL;

	if (head == NULL)
		head = newNumNode;
	else {
		numbernode *t = head;

		while (t->next)
			t = t->next;

		t->next = newNumNode;
	}
}

void NumberList::printNumList() {
	numbernode *t = head;
	int i = 1;

	while (t) {
		cout << "\t>> " << i++ << ". " << t->number << endl;
		t = t->next;
	}
}

numbernode* NumberList::removeFirst() {
	if (head) {
		numbernode *temp = head;
		head = head->next;
		temp->next = NULL;
		return temp;
	}
	else
		return NULL;
}

void NumberList::clear() {
	numbernode *t = head;

	while (t) {
		head = head->next;
		delete[] t->number;
		delete t;
		t = head;
	}
}

//---------------------------------------------------------------

void Tree::initialize() {
	root = NULL;
}

void Tree::add(treenode *newData) {
	treenode *newTNode, **t = &root;
	numbernode *temp;

	while (*t) {
		if (strcmp(newData->name, (*t)->name) > 0)
			t = &(*t)->right;
		else if (strcmp(newData->name, (*t)->name) < 0)
			t = &(*t)->left;
		else {
			while (temp = newData->phonenum.removeFirst())
				(*t)->phonenum.add(temp->number);
			cout << "New number or numbers have been added to the name " << (*t)->name << "." << endl;
			return;
		}
	}

	newTNode = new treenode;

	newTNode->name = new char[strlen(newData->name) + 1];
	strcpy(newTNode->name, newData->name);
	newTNode->phonenum.initialize();
	while (temp = newData->phonenum.removeFirst())
		newTNode->phonenum.add(temp->number);
	newTNode->left = NULL;
	newTNode->right = NULL;

	*t = newTNode;

	cout << "Record has been successfully created." << endl;
}

treenode* Tree::deleteNode(char *targetName) {
	treenode *t = root, *parent = root, *t2;
	int camefrom = 0;

	while (t) {
		if (strcmp(targetName, t->name) == 0)
			break;
		else {
			parent = t;
			if (strcmp(targetName, t->name) > 0) {
				t = t->right;
				camefrom = RIGHT;
			}
			else {
				t = t->left;
				camefrom = LEFT;
			}
		}
	}
	if (t) {
		if (!t->left || !t->right) {
			if (camefrom == LEFT)
				parent->left = t->left;
			else if (camefrom == RIGHT)
				parent->right = t->right;
			else {
				if (root->left)
					root = t->left;
				else if (root->right)
					root = t->right;
				else
					root = NULL;
			}
		}
		else {
			if (camefrom == LEFT)
				parent->left = t->right;
			else if (camefrom == RIGHT)
				parent->right = t->right;
			else
				root = t->right;

			t2 = t->right;
			while (t2->left)
				t2 = t2->left;

			t2->left = t->left;
		}

		return t;
	}
	else
		cout << "No record found with that name!" << endl;

	return NULL;
}

bool Tree::update(char *targetName, char *newName) {
	treenode *temp = deleteNode(targetName);

	delete[] temp->name;
	temp->name = new char[strlen(newName) + 1];
	strcpy(temp->name, newName);

	if (temp) {
		add(temp);
		return true;
	}
	else
		return false;
}

bool Tree::searchRecords(treenode *current, char *targetNamePart) {
	if (strcmp(targetNamePart, current->name) == 0) {
		cout << "Found: " << current->name << endl;
		current->phonenum.printNumList();
		return true;
	}
	else if (current->right && strcmp(targetNamePart, current->name) > 0)
		return searchRecords(current->right, targetNamePart);
	else if (current->left && strcmp(targetNamePart, current->name) < 0)
		return searchRecords(current->left, targetNamePart);

	return false;
}

void Tree::printTree(treenode *current, int *orderno) {
	if (current->left)
		printTree(current->left, orderno);

	cout << *orderno << ". " << current->name << endl;
	current->phonenum.printNumList();
	++(*orderno);

	if (current->right)
		printTree(current->right, orderno);
}

void Tree::clear(treenode *current) {
	if (current == NULL)
		return;

	if (current->left)
		clear(current->left);
	if (current->right)
		clear(current->right);

	current->phonenum.clear();
	delete[] current->name;
	if (current == root)
		root = NULL;
	delete current;
}

void Tree::printTreeRec(treenode *current, int indent) {
	if (current) {
		if (current->right)
			printTreeRec(current->right, indent + 8);

		for (int i = 0; i < indent; ++i)
			cout << ' ';
		cout << current->name << endl;

		if (current->left)
			printTreeRec(current->left, indent + 8);
	}
}