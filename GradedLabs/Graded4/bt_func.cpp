#include "binarytree.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

//----------------------TREE FUNCTIONS----------------------

void Tree::initialize() {
	createTree(&root, 0, N - 1);
}

void Tree::close() {
	removeTree(root);
}

void Tree::printTree() {
	printTreeRec(root, 0);
}

void Tree::createTree(TreeNode **current, int startindex, int endindex) {
	if (startindex <= endindex) {
		*current = new TreeNode;
		(*current)->val = rand() % 10000;
		(*current)->left = NULL;
		(*current)->right = NULL;

		if (startindex != endindex) {
			createTree(&((*current)->left), startindex, startindex + (endindex - startindex) / 2 - 1);
			createTree(&((*current)->right), startindex + (endindex - startindex) / 2 + 1, endindex);
		}
	}
}

void Tree::printTreeRec(TreeNode *current, int indent) {
	if (current) {
		if (current->right)
			printTreeRec(current->right, indent + 8);

		for (int i = 0; i < indent; ++i)
			cout << ' ';
		cout << current->val << endl;

		if (current->left)
			printTreeRec(current->left, indent + 8);
	}
}

void Tree::removeTree(TreeNode *current) {
	if (!(current->left) && !(current->right)) {
		delete current;
		return;
	}

	if (current->left)
		removeTree(current->left);
	if (current->right)
		removeTree(current->right);

	delete current;
}

bool Tree::InorderSearch(int searchVal, TreeNode *current, int *stepno) {
	if (current->left) {
		if (InorderSearch(searchVal, current->left, stepno))
			return true;
	}

	if (*stepno)
		cout << " -> " << current->val;
	else
		cout << current->val;

	if (current->val == searchVal) {
		cout << endl;
		return true;
	}

	++(*stepno);

	if (current->right) {
		if (InorderSearch(searchVal, current->right, stepno))
			return true;
	}

	return false;
}

bool Tree::BreadthFirstSearch(int searchVal) {
	TreeNode *t;
	Queue Q;
	bool flag = false;
	int stepno = 0;

	Q.initialize();
	Q.enqueue(root);

	while (!Q.isEmpty()) {
		t = Q.dequeue();

		if (t->left)
			Q.enqueue(t->left);

		if (t->right)
			Q.enqueue(t->right);

		if (t->val == searchVal) {
			flag = true;

			if (stepno)
				cout << t->val << endl;
			cout << "Ended at step no.: " << stepno << endl;

			break;
		}
		else
			cout << t->val << " -> ";

		++stepno;
	}

	Q.clear();

	return flag;
}

bool Tree::DepthFirstSearch(int searchVal) {
	TreeNode *t;
	Stack S;
	bool flag = false;
	int stepno = 0;

	S.initialize();
	S.push(root);

	while (!S.isEmpty()) {
		t = S.pop();

		if (t->left)
			S.push(t->left);

		if (t->right)
			S.push(t->right);

		if (t->val == searchVal) {
			flag = true;
			
			if (stepno)
				cout << t->val << endl;
			cout << "Ended at step no.: " << stepno << endl;

			break;
		}
		else
			cout << t->val << " -> ";

		++stepno;
	}

	S.clear();

	return flag;
}
//----------------------QUEUE FUNCTIONS----------------------

void Queue::initialize() {
	head = tail = NULL;
}

bool Queue::isEmpty() {
	if (head == NULL && tail == NULL)
		return true;
	else
		return false;
}

void Queue::enqueue(TreeNode *newNode) {
	Node *n = new Node;
	n->data = newNode;

	if (isEmpty()) {
		head = n;
		n->next = NULL;
	}
	else
		tail->next = n;
	tail = n;
}

TreeNode* Queue::dequeue() {
	Node *t;
	TreeNode* toReturn;

	if (isEmpty())
		return NULL;
	else {
		t = head;
		if (head == tail)
			head = tail = NULL;
		else
			head = head->next;
		toReturn = t->data;
		delete t;
		return toReturn;
	}
}

void Queue::clear() {
	while (dequeue());
}

//----------------------STACK FUNCTIONS----------------------

void Stack::initialize() {
	head = NULL;
}

bool Stack::isEmpty() {
	return (head == NULL);
}

void Stack::push(TreeNode *newNode) {
	Node *n = new Node;
	n->data = newNode;
	n->next = head;
	head = n;
}

TreeNode* Stack::pop() {
	Node *t;
	TreeNode *toReturn;

	if (isEmpty())
		return NULL;
	else {
		t = head;
		head = head->next;
		toReturn = t->data;
		delete t;
		return toReturn;
	}
}

void Stack::clear() {
	while (pop());
}