/* @Author
 * Student Name: Oguz Paksoy
 * Student ID: 150150111
 * Date: 15.10.2016
 */
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "patientrecord.h"

using namespace std;

void printMenu() {
	//function prints the user menu
	cout << endl;
	cout << "Please select the operation to perform and enter the operation code" << endl;
	cout << "(P) Print all of the patient records," << endl;
	cout << "(B) Search the data by the branch name," << endl;
	cout << "(C) Search the data by the polyclinic number," << endl;
	cout << "(I) Insert a new patient record," << endl;
	cout << "(R) Remove the patient record," << endl;
	cout << "(E) Exit the program," << endl;
	cout << "Your selection is: ";
}

bool makeDecision(RecordBook *PatRecs,char choice) {
	/* function to handle user decisions
	 * returns false if the choice is 'E' or 'e', returns true otherwise
	 */
	char buf[25];
	int input;
	patient_record temp;
	switch (choice) {
		case 'P': case 'p':
			//print all records
			cout << "************LIST************" << endl;
			PatRecs->printRecords();
			cout << "****************************" << endl;
			return true;
		case 'B': case 'b':
			//check for a proper input for branch
			do {
				cout << "Please enter the branch name: ";
				cin.sync();
				cin.getline(buf, 20);
			} while (strlen(buf) == 0);
			//print the matching results
			cout << "************LIST************" << endl;
			PatRecs->branchSearch(buf);
			cout << "****************************" << endl;
			return true;
		case 'C': case 'c':
			//get an input for the polyclinic number
			cout << "Please enter a polyclinic number: ";
			cin >> input;
			cin.ignore(1000, '\n');
			//print the matching results
			cout << "************LIST************" << endl;
			PatRecs->polySearch(input);
			cout << "****************************" << endl;
			return true;
		case 'I': case 'i':
			//check for a proper input for the patient's name
			do {
				cout << "Please enter the patient\'s name: ";
				cin.sync();
				cin.getline(buf, 20);
			} while (strlen(buf) == 0);
			//copy to temp
			strcpy(temp.name, buf);

			//check for a proper input for the doctor's name
			do {
				cout << "Please enter the doctor\'s name: ";
				cin.sync();
				cin.getline(buf, 20);
			} while (strlen(buf) == 0);
			//copy to temp
			strcpy(temp.doctorName, buf);

			//check for a proper input for the diagnosis
			do {
				cout << "Please enter the diagnosis: ";
				cin.sync();
				cin.getline(buf, 25);
			} while (strlen(buf) == 0);
			//copy to temp
			strcpy(temp.diagnosis, buf);

			//check for a proper input for the patient number
			do {
				cout << "Please enter a patient number between 1 and 9999 which is unique: ";
				cin >> input;
				cin.ignore(1000, '\n');
			} while (input < 1 || input > 9999 || PatRecs->existsInPatients(input) != -1);
			//copy to temp
			temp.patientNumber = input;

			//check for a proper input for the polyclinic number
			do {
				cout << "Please enter a polyclinic number between 1 and 999: ";
				cin >> input;
				cin.ignore(1000, '\n');
			} while (input < 1 || input > 999);
			//copy to temp
			temp.polyclinicNumber = input;

			//check for a proper input for the branch name
			do {
				cout << "Please enter the branch: ";
				cin.sync();
				cin.getline(buf, 20);
			} while (strlen(buf) == 0);
			//to uppercase
			for (char *p = buf; *p = toupper(*p); ++p);
			//copy to temp
			strcpy(temp.branchName, buf);
			//if temp could be inserted, inform the user
			if (PatRecs->insertRecord(&temp))
				cout << "Successfully inserted record into the record book." << endl;
			//else print an error message with possible causes of error
			else {
				cout << "ERROR: Insertion failed. Please make sure that you\'re not attempting the following: " << endl;
				cout << "-> Inserting a patient record into a full polyclinic," << endl;
				cout << "-> Inserting a patient record with a new polyclinic number when the branch already has 2 polyclinics," << endl;
				cout << "-> Inserting a patient record with a polyclinic number that doesn\'t belong to the specified branch." << endl;
			}
			return true;
		case 'R': case 'r':
			//get an input for the patient number
			cout << "Please enter the patient number to be deleted: ";
			cin >> input;
			cin.ignore(1000, '\n');
			//if input removed properly, inform the user
			if (PatRecs->removeRecord(input))
				cout << "Removed patient " << input << " successfully." << endl;
			//else print an error message
			else
				cout << "ERROR: A patient record with that patient number doesn\'t exist." << endl;
			return true;
		case 'E': case 'e':
			//exit program
			cout << "Goodbye!" << endl;
			return false;
		default:
			//invalid operation code
			cout << "Please enter a valid operation code." << endl;
			return true;
	}
}

int main() {
	char choice;
	//initialize RecordBook
	RecordBook PatientRecords;
	PatientRecords.initialize();
	//read from database.txt
	PatientRecords.readFile();

	//until user exits
	do {
		//print menu and read their choice
		printMenu();
		cin >> choice;
		cin.ignore(1000, '\n');
	} while (makeDecision(&PatientRecords, choice));

	//write onto database.txt
	PatientRecords.writeFile();
	return 0;
}
