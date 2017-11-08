#define N 31

struct TreeNode {
	int val;
	TreeNode *left;
	TreeNode *right;
};

struct Tree {
	TreeNode *root;

	void initialize();
	void close();
	void printTree();
	void createTree(TreeNode **current, int startindex, int endindex);
	void removeTree(TreeNode *current);
	void printTreeRec(TreeNode *current, int indent);
	bool InorderSearch(int searchVal, TreeNode *current, int *stepno);
	bool BreadthFirstSearch(int searchVal);
	bool DepthFirstSearch(int searchVal);
};

struct Node {
	TreeNode *data;
	Node *next;
};

struct Queue {
	Node *head, *tail;

	void initialize();
	bool isEmpty();
	void enqueue(TreeNode *newNode);
	TreeNode* dequeue();
	void clear();
};

struct Stack {
	Node *head;

	void initialize();
	void push(TreeNode *newNode);
	TreeNode* pop();
	bool isEmpty();
	void clear();
};