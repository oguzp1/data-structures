#define N 9

struct TreeNode {
	int val;
	TreeNode *left, *right;
};

struct Tree {
	TreeNode *root;
	int depth;
	int leaves;

	void createTree();
	void removeTree(TreeNode *current);
	void printPreorder(TreeNode *start);
	void printInorder(TreeNode *start);
	void printPostorder(TreeNode *start);
	int findMax(TreeNode *current);
	int findMin(TreeNode *current);
	int findNumNode();
	int findNumLeaf(TreeNode *current);
	int calculateDepth();
	int calculateSum();
	int calculateAverage();
};