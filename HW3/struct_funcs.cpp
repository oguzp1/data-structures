/* @Author
 * Student Name: Oguz Paksoy
 * Student ID: 150150111
 * Date: 25.12.2016
 */
#include <climits>
#include <cstring>
#include "structs.h"

void stack::initialize() {
	head = NULL;
}

void stack::push(char *data) {
	node *newnode = new node;
	newnode->str = new char[strlen(data) + 1];
	strcpy(newnode->str, data);
	newnode->next = head;
	head = newnode;
}

char* stack::pop() {
	node *n = head;
	char *toReturn = n->str;;

	head = head->next;
	delete n;

	return toReturn;
}

bool stack::isEmpty() {
	return (head == NULL);
}

void stack::clear() {
	node *temp;

	while (head) {
		temp = head;
		head = head->next;
		delete[] temp->str;
		delete temp;
	}
}

//-----------------------------------------------

void queue::initialize() {
	tail = NULL;
}

bool queue::isEmpty() {
	return (tail == NULL);
}

void queue::enqueue(char *data) {
	node *newnode = new node;
	newnode->str = new char[strlen(data) + 1];
	strcpy(newnode->str, data);
	
	if (tail) {
		newnode->next = tail->next;
		tail->next = newnode;
		tail = tail->next;
	}
	else {
		tail = newnode;
		tail->next = tail;
	}
}

char* queue::dequeue() {
	node *toDelete = tail->next;
	char *toReturn = toDelete->str;

	if (tail == toDelete)
		tail = NULL;
	else
		tail->next = toDelete->next;

	delete toDelete;

	return toReturn;
}

void queue::clear() {
	node *temp;

	while (tail != tail->next) {
		temp = tail;
		tail = tail->next;
		delete[] temp->str;
		delete temp;
	}
	temp = tail;
	tail = NULL;
	delete[] temp->str;
	delete temp;
}

//-----------------------------------------------

//initializes with a null character
void String::initialize() {
	strNode *n = new strNode;
	n->c = '\0';
	n->next = NULL;
	head = n;
	tail = n;
	strSize = 1;
}

//returns the total string produced
char* String::getString() {
	char *toReturn = new char[strSize], *traverse = toReturn;
	strNode *t = head;
	
	while (t) {
		*traverse = t->c;
		++traverse;
		t = t->next;
	}

	return toReturn;
}

//adds to the end of the string
void String::addToEnd(char *str) {
	for (char *traverse = str; *traverse; ++traverse) {
		//if first character, write onto the last node
		if (traverse == str)
			tail->c = *traverse;
		//else generate nodes
		else {
			strNode *n = new strNode;
			n->c = *traverse;
			tail->next = n;
			n->next = NULL;
			tail = n;
		}
		++strSize;
	}
	//generate a final node for the null character
	strNode *finalchar = new strNode;
	finalchar->c = '\0';
	tail->next = finalchar;
	finalchar->next = NULL;
	tail = finalchar;
}

void String::clear() {
	strNode *t = head;

	while (head->c) {
		head = head->next;
		delete t;
		t = head;
	}

	strSize = 1;
}

void String::closeString() {
	clear();
	delete head;
	head = NULL;
	tail = NULL;
	strSize = 0;
}