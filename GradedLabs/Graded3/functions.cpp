#include <iostream>
#include <ctime>
#include "tree.h"

using namespace std;

void placeInNodes(TreeNode *nodeArr[N]) {
	int randomnum, place[N] = { 0 }, arr[N], remaining = N;

	for (int i = 0; i < N; ++i) {
		randomnum = rand() % remaining;
		for (int j = 0; j < N; ++j) {
			if (place[j])
				continue;

			if (randomnum) {
				--randomnum;
				continue;
			}

			arr[j] = i + 1;
			--remaining;
			place[j] = 1;
			break;
		}
	}

	for (int i = 0; i < N; ++i) {
		nodeArr[i] = new TreeNode;
		nodeArr[i]->val = arr[i];
		nodeArr[i]->left = NULL;
		nodeArr[i]->right = NULL;
	}
}

void Tree::createTree() {
	TreeNode ***allnodes, *nodeArr[N];
	int count = 0, pow = 1, cursor = 0;
	for (int i = pow; i < N; i += pow) {
		pow *= 2;
		++count;
	}
	depth = count + 1;

	placeInNodes(nodeArr);

	allnodes = new TreeNode**[count + 1];
	pow = 1;
	for (int i = 0; i < count + 1; ++i) {
		allnodes[i] = new TreeNode*[pow];
		for (int j = 0; j < pow; ++j) {
			if (cursor < N) {
				allnodes[i][j] = nodeArr[cursor];
				++cursor;
			}
			else {
				allnodes[i][j] = NULL;
			}
		}
		pow *= 2;
	}

	root = allnodes[0][0];
	
	for (int i = 0; i < count; ++i) {
		for (int j = 0; j < i + 1; ++j) {
			allnodes[i][j]->left = allnodes[i + 1][j * 2];
			allnodes[i][j]->right = allnodes[i + 1][j * 2 + 1];
		}
	}
	
	for (int i = 0; i < count + 1; ++i)
		delete allnodes[i];
	delete allnodes;
}

void Tree::removeTree(TreeNode *current) {
	
}

void Tree::printPreorder(TreeNode* start) {
	if (start == NULL)
		return;

	cout << start->val << " ";
	printPreorder(start->left);
	printPreorder(start->right);
}

void Tree::printInorder(TreeNode* start) {
	if (start == NULL)
		return;

	printInorder(start->left);
	cout << start->val << " ";
	printInorder(start->right);
}

void Tree::printPostorder(TreeNode* start) {
	if (start == NULL) 
		return;

	printPostorder(start->left);
	printPostorder(start->right);
	cout << start->val << " ";
}

int Tree::findMax(TreeNode *current) {
	static int max = 0;

	if (current == NULL)
		return 0;

	if (max < current->val)
		max = current->val;

	findMax(current->left);
	findMax(current->right);

	return max;
}

int Tree::findMin(TreeNode *current) {
	static int min = N + 1;

	if (current == NULL)
		return N + 1;

	if (min > current->val)
		min = current->val;

	findMin(current->left);
	findMin(current->right);

	return min;
}

int Tree::calculateDepth() {
	return depth;
}

int Tree::findNumNode() {
	return N;
}

int Tree::findNumLeaf(TreeNode *current) {
	if (current->left == NULL && current->right == NULL) {
		return 1;
	}
	else {
		return findNumLeaf(current->left) + findNumLeaf(current->right);
	}
}

int Tree::calculateSum() {
	return N*(N + 1) / 2;
}

int Tree::calculateAverage() {
	return (N + 1) / 2;
}