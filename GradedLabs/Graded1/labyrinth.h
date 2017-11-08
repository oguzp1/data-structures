#define ROWCOUNT 14
#define COLCOUNT 21

struct Coordinate {
	int i, j;
};

struct LocationData {
	Coordinate coords;
	bool upAvailable, downAvailable, leftAvailable, rightAvailable;

	int totalWays();
};

struct TravelStack {
	Coordinate stack[ROWCOUNT*COLCOUNT];
	int cursor;

	void initialize();
	bool isEmpty();
	void push(Coordinate *nc);
	Coordinate* pop();
};

struct NodeStack {
	LocationData stack[ROWCOUNT*COLCOUNT];
	int cursor;

	void initialize();
	bool isEmpty();
	void push(LocationData *nld);
	LocationData* pop();
};

bool coordsEqual(Coordinate *c1, Coordinate *c2);