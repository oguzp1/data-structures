#define NAME_LENGTH 30
#define NUM_LENGTH 20
#define LEFT 1
#define RIGHT 2

struct numbernode {
	char *number;
	numbernode *next;
};

struct NumberList {
	numbernode *head;

	void initialize();
	void add(char *newNumber);
	void printNumList();
	numbernode* removeFirst();
	void clear();
};

struct treenode {
	char *name;
	NumberList phonenum;
	treenode *left;
	treenode *right;
};

struct Tree {
	treenode *root;

	void initialize();
	void add(treenode *newData);
	treenode* deleteNode(char *targetName);
	bool update(char *targetName, char *newName);
	bool searchRecords(treenode *current, char *targetNamePart);
	void printTree(treenode *current, int *orderno);
	void clear(treenode *current);
	void printTreeRec(treenode *current, int indent);
};