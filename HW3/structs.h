/* @Author
 * Student Name: Oguz Paksoy
 * Student ID: 150150111
 * Date: 25.12.2016
 */
#define MAX_EXPRESSION_LENGTH 200
#define MAX_NUMBER_LENGTH 20

struct node {
	char *str;
	node *next;
};

struct stack {
	node *head;

	void initialize();
	void push(char *data);
	char* pop();
	bool isEmpty();
	void clear();
};

struct queue {
	node *tail;

	void initialize();
	bool isEmpty();
	void enqueue(char *data);
	char* dequeue();
	void clear();
};

struct strNode {
	char c;
	strNode *next;
};

struct String {
	strNode *head;
	strNode *tail;
	int strSize;

	void initialize();
	char* getString();
	void addToEnd(char *str);
	void clear();
	void closeString();
};