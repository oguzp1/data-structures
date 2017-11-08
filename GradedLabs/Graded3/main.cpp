#include <iostream>
#include "tree.h"
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
	Tree t;
	t.createTree();
	cout << t.findMax(t.root) << endl;
	cout << t.findMin(t.root) << endl;
	cout << t.findNumLeaf(t.root) << endl;
	cout << t.findNumNode() << endl;
	getchar();
	return 0;
}