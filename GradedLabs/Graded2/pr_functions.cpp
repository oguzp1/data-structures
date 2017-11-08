#include "maze.h"

void CellStack::initialize() {
	cursor = 0;
}

void CellStack::push(int new_i, int new_j) {
	i_stack[cursor] = new_i;
	j_stack[cursor++] = new_j;
}

void CellStack::pop(int *popped_ipos, int *popped_jpos) {
	*popped_ipos = i_stack[--cursor];
	*popped_jpos = j_stack[cursor];
}

bool CellStack::isEmpty() {
	return (cursor == 0);
}

void CellStack::clearStack() {
	int i, j;
	while (!isEmpty())
		pop(&i, &j);
}