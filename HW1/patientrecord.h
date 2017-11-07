/* @Author
 * Student Name: Oguz Paksoy
 * Student ID: 150150111
 * Date: 15.10.2016
 */
#define MAX_RECORD_COUNT 200
#define NO_EXTRA_CONDITION true

struct patient_record {
	char name[20], doctorName[20], diagnosis[25];
	int patientNumber, polyclinicNumber;
	char branchName[20];
};

struct RecordBook {
	//VARIABLES
	patient_record allRecords[MAX_RECORD_COUNT]; //array of patient_record structures
	int usedPolyNums[MAX_RECORD_COUNT / 10][2], //array to keep track of used "polyclinicNumber"s (ordered by branchName and polyclinicNumber [0-1])
		last_allocated, //int to keep track of the allocated element of the array that is closest to the end
		noOfBranches; //int to keep track of the number of registered branches
	char usedBranches[MAX_RECORD_COUNT / 10][20]; //array to keep track of used branchNames (ordered by branchName)
	//FUNCTIONS
	void initialize();
	void readFile();
	void writeFile();
	void printRecords();
	void branchSearch(char *branchToSearch);
	void polySearch(int polyToSearch);
    void copyRecord(patient_record *destination, patient_record *source);
    bool allocate(int index);
	void deallocate(int index);
	bool removeRecord(int patient);
	int whereTo(int index);
	bool insertRecord_polyInUse(patient_record *newRecord, int inBranches, int inPoly);
	bool insertRecord_branchInUse(patient_record *newRecord, int inBranches);
	bool insertRecord_newBranch(patient_record *newRecord);
    bool insertRecord(patient_record *newRecord);
	int existsInPoly(int polyNo);
	int existsInBranches(char *branch);
	int existsInPatients(int patient);
};
