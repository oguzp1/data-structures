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

void RecordBook::initialize() {
	//initialization function
	for (int i = 0; i < MAX_RECORD_COUNT; ++i)
		//condition for emptiness, if the first char of name is '\0' the element is considered to be empty
		(allRecords + i)->name[0] = '\0';
	//index based variable, initialized to -1 to keep track of the final index that has been allocated easier
	last_allocated = -1;
	//no of used branches
	noOfBranches = 0;
	//initialize the branch and polyclinic arrays
	for (int i = 0; i < MAX_RECORD_COUNT / 10; ++i) {
		usedBranches[i][0] = '\0';
		usedPolyNums[i][0] = -1;
		usedPolyNums[i][1] = -1;
	}
}

void RecordBook::readFile() {
	//function inserts each element read from the database into the array
	FILE *fr = fopen("database.txt", "r");
	patient_record temp;
	char line[200];
	for (int i = 0; fgets(line, sizeof(line), fr); ++i) {
		sscanf(line, "%19[^\t]\t%19[^\t]\t%24[^\t]\t%d\t%d\t%19[^\r\n]%*[\r\n]", &(temp.name),
                                                                                 &(temp.doctorName),
                                                                                 &(temp.diagnosis),
                                                                                 &(temp.patientNumber),
                                                                                 &(temp.polyclinicNumber),
                                                                                 &(temp.branchName));
		insertRecord(&temp);
	}
	fclose(fr);
}

void RecordBook::writeFile() {
	//function writes every non-empty record onto the database
	FILE *fw = fopen("database.txt", "w");
	for (int i = 0; i <= last_allocated; ++i) {
		if ((allRecords + i)->name[0] != '\0') {
			fprintf(fw, "%s\t%s\t%s\t%d\t%d\t%s\n", (allRecords + i)->name,
													(allRecords + i)->doctorName,
													(allRecords + i)->diagnosis,
													(allRecords + i)->patientNumber,
													(allRecords + i)->polyclinicNumber,
													(allRecords + i)->branchName);
		}
	}
	fclose(fw);
}

void RecordBook::printRecords() {
	//function prints all non-empty records
	int i, listitem = 1;
	for (i = 0; i <= last_allocated; ++i) {
		if (((allRecords + i)->name[0] != '\0')) {
			printf("%3d. %20s\t%20s\t%25s\t%4d\t%3d\t%20s\n", listitem,
															(allRecords + i)->name,
															(allRecords + i)->doctorName,
															(allRecords + i)->diagnosis,
															(allRecords + i)->patientNumber,
															(allRecords + i)->polyclinicNumber,
															(allRecords + i)->branchName);
			++listitem;
		}
	}
}

void RecordBook::branchSearch(char *branchToSearch) {
	/* function prints all non-empty records that match the branchName searched
	 * case insensitive
	 */
	int i, listitem = 1;
	//to uppercase
	for (char *p = branchToSearch; *p = toupper(*p); ++p);

	for (i = 0; i <= last_allocated; ++i) {
		if (((allRecords + i)->name[0] != '\0') && strcmp(branchToSearch, (allRecords + i)->branchName) == 0) {
			printf("%3d. %20s\t%20s\t%25s\t%4d\t%3d\t%20s\n", listitem,
															  (allRecords + i)->name,
															  (allRecords + i)->doctorName,
															  (allRecords + i)->diagnosis,
															  (allRecords + i)->patientNumber,
															  (allRecords + i)->polyclinicNumber,
															  (allRecords + i)->branchName);
			++listitem;
		}
	}
}

void RecordBook::polySearch(int polyToSearch) {
	//function prints all non-empty records that match the polyclinicNumber searched
	int i, listitem = 1;
	for (i = 0; i <= last_allocated; ++i) {
		if (((allRecords + i)->name[0] != '\0') && (polyToSearch == (allRecords + i)->polyclinicNumber)) {
			printf("%3d. %20s\t%20s\t%25s\t%4d\t%3d\t%20s\n", listitem,
															  (allRecords + i)->name,
															  (allRecords + i)->doctorName,
															  (allRecords + i)->diagnosis,
															  (allRecords + i)->patientNumber,
															  (allRecords + i)->polyclinicNumber,
															  (allRecords + i)->branchName);
			++listitem;
		}
	}
}

void RecordBook::copyRecord(patient_record *destination, patient_record *source) {
	//function copies the contents of a patient_record to another
	strcpy(destination->name, source->name);
	strcpy(destination->doctorName, source->doctorName);
	strcpy(destination->diagnosis, source->diagnosis);
	destination->patientNumber = source->patientNumber;
	destination->polyclinicNumber = source->polyclinicNumber;
	strcpy(destination->branchName, source->branchName);
}

bool RecordBook::allocate(int index) {
	/* function checks if the array has room for 10 more records, and if it does,
	   it slides the patient_records so that the following 10 items from the given index
	   are empty
	 * also considers the case where there is nothing to move
	 * returns false if there's no room, returns true otherwise
	 */
	if (last_allocated == MAX_RECORD_COUNT - 1)
		return false;

	if (last_allocated != -1) {
		for (int i = last_allocated; i >= index; --i)
			copyRecord((allRecords + i + 10), (allRecords + i));

		for (int i = index; i < index + 10; ++i)
			(allRecords + i)->name[0] = '\0';
	}
	last_allocated += 10;
	return true;
}

void RecordBook::deallocate(int index) {
	/* function slides the entries after this index + 9 so that
	   the 10 items following "index" are overwritten
	 * works in the case where the items to be removed are the last items
	   allocated
	 */
	for (int i = index; i <= last_allocated; ++i)
		copyRecord((allRecords + i), (allRecords + i + 10));

	for (int i = last_allocated; i > last_allocated - 10; --i)
		(allRecords + i)->name[0] = '\0';

	last_allocated -= 10;
}

bool RecordBook::removeRecord(int patient) {
	/* function checks if a number with the given patientNumber exists, and if it does,
	   it erases the record, as well as it's usedBranches and usedPolyNums records
	   if there isn't any non-empty elements left in the allocated section our record
	   resided in
	 * returns false if the record of the patient doesn't exist, returns true otherwise
	 */
	bool flag;
	int startOfSection, inPoly, locationIndex = existsInPatients(patient);
	//if a record with that patientNumber doesn't exist, return false
	if (locationIndex == -1)
		return false;
	//else get the location in the usedPolyNums array
	inPoly = existsInPoly((allRecords + locationIndex)->polyclinicNumber);
	//find where the branch containing the polyclinic starts in the patient_records array
	startOfSection = whereTo(inPoly);
	//find the start of the polyclinic records
	if ((allRecords + locationIndex)->polyclinicNumber != (allRecords + startOfSection)->polyclinicNumber)
		startOfSection += 10;
	//render the record empty
	(allRecords + locationIndex)->name[0] = '\0';
	//search for non-empty records within the polyclinic
	flag = true;
	for (int i = startOfSection; i < startOfSection + 10; ++i) {
		if ((allRecords + i)->name[0] != '\0') {
			flag = false;
			break;
		}
	}
	//if polyclinic empty
	if (flag) {
		//if there's only one polyclinic in the branch
		if (usedPolyNums[inPoly][1] == -1) {
			//move the items in usedBranches and usedPolynNums up to overwrite
			for (int i = inPoly; i < noOfBranches - 1; ++i) {
				strcpy(usedBranches[i], usedBranches[i + 1]);
				usedPolyNums[i][0] = usedPolyNums[i + 1][0];
				usedPolyNums[i][1] = usedPolyNums[i + 1][1];
			}
			//empty the last element
			usedBranches[noOfBranches - 1][0] = '\0';
			usedPolyNums[noOfBranches - 1][0] = -1;
			usedPolyNums[noOfBranches - 1][1] = -1;
			//decrement noOfBranches
			--noOfBranches;
		}
		//if there are 2 polyclinics in the branch
		else {
			//if the record exists in the first polyclinic of the branch, move the second one to the first place
			if ((allRecords + locationIndex)->polyclinicNumber == usedPolyNums[inPoly][0])
				usedPolyNums[inPoly][0] = usedPolyNums[inPoly][1];
			//erase whatever is written on the second polyclinic of the branch
			usedPolyNums[inPoly][1] = -1;
		}
		//deallocate unused region
		deallocate(startOfSection);
	}
	//return true when done
	return true;
}

int RecordBook::whereTo(int index) {
	/* function calculates how many array elements were allocated
	   before the given branch order index (the order in usedBranches)
	 * returns the first index that should be written on or deleted
	 */
	int toIndex = 0;
	for (int i = 0; i < index; ++i) {
		if (usedPolyNums[i][0] != -1) {
			++toIndex;
			if (usedPolyNums[i][1] != -1)
				++toIndex;
		}
	}
	toIndex *= 10;
	return toIndex;
}

bool RecordBook::insertRecord_polyInUse(patient_record *newRecord, int inBranches, int inPoly) {
	/* function inserts a record with a branchName and a polyclinicNumber that was used before
	 * returns false if the polyclinic is full or the polyclinic doesn't belong to that branch
	   returns true otherwise
	 */
	//if polyclinicNumber doesn't match the branchName it was assigned to, return false
	if (inPoly != inBranches)
		return false;
	//decide where to look
	int startOfPoly = whereTo(inPoly);
	//determine the order of the polyclinicNumber
	if (usedPolyNums[inPoly][0] < newRecord->polyclinicNumber)
		startOfPoly += 10;
	//else find an empty spot in the polyclinic, if able return true
	for (int i = startOfPoly; i < startOfPoly + 10; ++i) {
		if ((allRecords + i)->name[0] == '\0') {
			copyRecord((allRecords + i), newRecord);
			return true;
		}
	}
	//if the polyclinic is full, return false
	return false;
}

bool RecordBook::insertRecord_branchInUse(patient_record *newRecord, int inBranches) {
	/* function inserts a record with a branchName that was used before,
	   and a polyclinicNumber that wasn't used before
	 * returns false if the branch already has 2 polyclinics or there isn't enough memory
	   returns true otherwise
	 */
	int newPoly;
	//if branch has 2 polyclinics and a new one is to be inserted, return false
	if (usedPolyNums[inBranches][1] != -1)
		return false;
	//determine where to write on the array by counting used polyclinicNumbers before the index
	newPoly = whereTo(inBranches);
	//if polyclinicNumber is smaller than the other polyclinic's number
	if (usedPolyNums[inBranches][0] > newRecord->polyclinicNumber) {
		//insert new polyclinic before the old one
		usedPolyNums[inBranches][1] = usedPolyNums[inBranches][0];
		usedPolyNums[inBranches][0] = newRecord->polyclinicNumber;
	}
	//if polyclinicNumber is larger than the other polyclinic's number
	else {
		//insert new clinic after the old one
		newPoly += 10;
		usedPolyNums[inBranches][1] = newRecord->polyclinicNumber;
	}
	//allocate memory at the appropriate index and insert record
	if (allocate(newPoly)) {
		copyRecord((allRecords + newPoly), newRecord);
		//if able return true
		return true;
	}
	//if allocation fails, return false
	return false;
}

bool RecordBook::insertRecord_newBranch(patient_record *newRecord) {
	/* function inserts a record with a unique branchName and polyclinic Number
	 * returns false if there isn't enough space, returns true otherwise
	 */
	int i = 0, newPoly;
	//get where the new polyclinic and branch should be placed
	for (; (i < noOfBranches) && (strcmp(newRecord->branchName, usedBranches[i]) > 0); ++i);
	//find index for the new data to be written
	newPoly = whereTo(i);
	//if allocation of records fail, return false
	if (allocate(newPoly) == false)
		return false;
	//else move used branches and polyclinics accordingly
	for (int j = noOfBranches; j > i; --j) {
		strcpy(usedBranches[j], usedBranches[j - 1]);
		usedPolyNums[j][0] = usedPolyNums[j - 1][0];
		usedPolyNums[j][1] = usedPolyNums[j - 1][1];
	}
	strcpy(usedBranches[i], newRecord->branchName);
	usedPolyNums[i][0] = newRecord->polyclinicNumber;
	usedPolyNums[i][1] = -1;
	//increment number of branches
	++noOfBranches;
	//copy contents of the first record of its kind into the patient_struct array
	copyRecord((allRecords + newPoly), newRecord);
	//if able return true
	return true;
}

bool RecordBook::insertRecord(patient_record *newRecord) {
	/* function inserts a record to an appropriate place in the array
	 * also checks for polyclinic dupes, full records and max. polyclinics per branch
	 * returns true if insertion was successful, otherwise returns false
	 */
	int inPoly = existsInPoly(newRecord->polyclinicNumber),
		inBranches = existsInBranches(newRecord->branchName);

	//if polyclinicNumber unused
	if (inPoly == -1) {
		//if branchName unused
		if (inBranches == -1)
			return insertRecord_newBranch(newRecord);
		//if branchName is used
		else
			return insertRecord_branchInUse(newRecord, inBranches);
	}
	//if polyclinicNumber is used
	else
		return insertRecord_polyInUse(newRecord, inBranches, inPoly);
}

int RecordBook::existsInPoly(int polyNo) {
	/* function checks if the polyclinicNumber was used before
	 * returns the index of its branch if it was, returns -1 if it wasn't
	 */
	for (int i = 0; i < noOfBranches; ++i) {
		if (usedPolyNums[i][0] == polyNo || usedPolyNums[i][1] == polyNo)
			return i;
	}
	return -1;
}

int RecordBook::existsInBranches(char *branch) {
	/* function checks if the branchName was used before
	 * returns the index of its branch if it was, returns -1 if it wasn't
	 */
	for (int i = 0; i < noOfBranches; ++i) {
		if (strcmp(usedBranches[i], branch) == 0)
			return i;
	}
	return -1;
}

int RecordBook::existsInPatients(int patient) {
	/* function checks if the patientNumber was used before in non-empty patient_records
	 * returns the index of the array element if it was, returns -1 if it wasn't
	 */
	for (int i = 0; i <= last_allocated; ++i) {
		if ((allRecords->name[0] != '\0') && (patient == (allRecords + i)->patientNumber))
			return i;
	}
	return -1;
}
