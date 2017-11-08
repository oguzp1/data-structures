#include <iostream>
#include "labyrinth.h"

using namespace std;

bool coordsEqual(Coordinate *c1, Coordinate *c2) {
	return ((c1->i == c2->i) && (c1->j == c2->j));
}

int LocationData::totalWays() {
	int counter = 0;

	if (upAvailable) ++counter;
	if (downAvailable) ++counter;
	if (leftAvailable) ++counter;
	if (rightAvailable) ++counter;

	return counter;
}


void TravelStack::initialize() {
	cursor = 0;
}

bool TravelStack::isEmpty() {
	if (cursor == 0)
		return true;
	else
		return false;
}

void TravelStack::push(Coordinate *nc) {
	stack[cursor] = *nc;
	++cursor;
}

Coordinate* TravelStack::pop() {
	if (isEmpty())
		return NULL;

	--cursor;
	return (stack + cursor);
}

void NodeStack::initialize() {
	cursor = 0;
}

bool NodeStack::isEmpty() {
	if (cursor == 0)
		return true;
	else
		return false;
}

void NodeStack::push(LocationData *nld) {
	(stack + cursor)->coords = nld->coords;
	(stack + cursor)->upAvailable = nld->upAvailable;
	(stack + cursor)->downAvailable = nld->downAvailable;
	(stack + cursor)->leftAvailable = nld->leftAvailable;
	(stack + cursor)->rightAvailable = nld->rightAvailable;
	++cursor;
}

LocationData* NodeStack::pop() {
	if (isEmpty())
		return NULL;

	--cursor;
	return (stack + cursor);
}