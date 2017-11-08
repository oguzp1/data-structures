#define MAZESIZE 20
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

struct cell {
	bool left, right, up, down;
	char val;
	bool visited;
};

struct CellStack {
	int i_stack[MAZESIZE*MAZESIZE];
	int j_stack[MAZESIZE*MAZESIZE];
	int cursor;

	void initialize();
	void push(int new_i, int new_j);
	void pop(int *popped_ipos, int *popped_jpos);
	bool isEmpty();
	void clearStack();
};