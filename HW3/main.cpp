/* @Author
 * Student Name: Oguz Paksoy
 * Student ID: 150150111
 * Date: 25.12.2016
 */
#include <iostream>
#include <climits>
#include <cstdio>
#include <cstring>
#include "structs.h"

using namespace std;

bool evaluate(char *inputStr, long *ans);

int main(int argc, char *argv[]) {
	char buffer[MAX_EXPRESSION_LENGTH + 1], tempstr[MAX_NUMBER_LENGTH + 1], *dequeued;
	long ans;
	//initialize queue
	queue ansQueue;
	ansQueue.initialize();

	//batch mode
	if (argc > 1) {
		//read file argv[1]
		FILE *fr = fopen(argv[1], "r");
		//if file was not found, exit
		if (!fr) {
			cout << "File does not exist." << endl << endl;
			return 0;
		}

		//while there are new lines
		while (fgets(buffer, MAX_EXPRESSION_LENGTH, fr)) {
			//evaluate infix expression, check for syntax errors
			//if the expression had proper syntax
			if (evaluate(buffer, &ans)) {
				//get a string associated with ans
				if (ans == LONG_MAX)
					strcpy(tempstr, "inf");
				else if (ans == LONG_MIN)
					strcpy(tempstr, "-inf");
				else
					sprintf(tempstr, "%ld", ans);
				//enqueue
				ansQueue.enqueue(tempstr);
			}
			else {
				cout << endl << "Syntax Error." << endl << endl;
				return 0;
			}
		}

		//print and delete elements of the queue
		cout << endl << "Answer queue: ";
		dequeued = ansQueue.dequeue();
		cout << dequeued << " ";
		delete[] dequeued;
		while (!ansQueue.isEmpty()) {
			cout << "-> ";
			dequeued = ansQueue.dequeue();
			cout << dequeued << " ";
			delete[] dequeued;
		}
		cout << endl << endl;

		return 0;
	}

	//initialize interpreter
	cout << endl << "Initializing the interpreter..." << endl << endl << ">> ";

	//get string input
	fgets(buffer, MAX_EXPRESSION_LENGTH, stdin);
	//while not exit
	while (strncmp(buffer, "exit", 4) != 0) {
		//evaluate infix expression, check for syntax errors
		//if the expression had proper syntax
		if (evaluate(buffer, &ans)) {
			//print the returned postfix string
			cout << endl << "Postfix string: " << buffer << endl;
			//get a string associated with ans
			if (ans == LONG_MAX)
				strcpy(tempstr, "inf");
			else if (ans == LONG_MIN)
				strcpy(tempstr, "-inf");
			else
				sprintf(tempstr, "%ld", ans);
			cout << "Answer: " << tempstr << endl << endl << ">> ";
			ansQueue.enqueue(tempstr);
		}
		else
			cout << endl << "Syntax Error." << endl << endl << ">> ";
		//get string input
		fgets(buffer, MAX_EXPRESSION_LENGTH, stdin);
	}
		
	//print and delete elements of the queue
	if (!ansQueue.isEmpty()) {
		cout << endl << "Terminating the interpreter..." << endl << "Answer queue: ";
		dequeued = ansQueue.dequeue();
		cout << dequeued << " ";
		delete[] dequeued;
		while (!ansQueue.isEmpty()) {
			cout << "-> ";
			dequeued = ansQueue.dequeue();
			cout << dequeued << " ";
			delete[] dequeued;
		}
		cout << endl << endl;
	}
	else
		cout << endl;

	return 0;
}