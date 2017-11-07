/* @Author
 * Student Name: Oguz Paksoy
 * Student ID: 150150111
 * Date: 09.11.2016 */
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "musiclist.h"

using namespace std;

//asks the user how to play the songs, and plays them
void play(list *mainList, int indexSelected, char operationSelected);
//prints all playlists and lets the user choose one, then operates given commands on that playlists
void printList(list *mainList);
//asks the user which playlists to add a song to, unless called from printList(), and then adds the specified song in the SORTED playlist
void addSong(list *mainList, int indexSelected);
//asks the user which playlist to remove from, unless called from printList(), and then removes the specified song
void removeSong(list *mainList, int indexSelected);
//asks the user how to create the new playlist and creates it
void createPlaylist(list *mainList);
//asks the user which playlist to delete, unless called from printList()
void deletePlaylist(list *mainList, int indexSelected);
//saves playlists to file
void writeToFile(list *mainList);
//deallocates list data, saves to file
void exit(list *mainList);
//prints the main menu of the progran
void printMainMenu();
//main decision making function that routes the main menu functions
bool makeDecision(list *mainList, char choice);
//erases extra characters and newline in input stream after cin and getchar uses
void flushInputChars();

int main() {
	//dynamically allocate memory for the new list
	list *mainList = new list;
	char choice;

	//initialize new list
	mainList->initializeList();
	
	//do while makeDecision() function returns the exit call
	do {
		//print main menu each time
		printMainMenu();
		//get user's choice
		choice = getchar();
		flushInputChars();
	} while (makeDecision(mainList, choice));

	//delete the allocated memory for the list
	delete mainList;

	//exit program
    return 0;
}

void play(list *mainList, int indexSelected, char operationSelected) {
	int index;
	char operation, ch;
	bool inputOkay;
	song_node *temp;
	playlist *p;

	if (operationSelected == '\0') {
		//if an operation isn't specified, let the user choose
		//print options and take inputs until the user enters a valid one
		inputOkay = false;
		do {
			cout << "PLAY >" << endl;
			cout << "Choose an option " << endl;
			cout << "L: Play a playlist" << endl;
			cout << "S: Start playing a playlist from a specific song" << endl;
			cout << "P: Play a single song" << endl;
			cout << "E: Exit" << endl << endl;
			cout << "Enter a choice (L,S,P,E): ";
			operation = getchar();
			flushInputChars();
			switch (operation) {
				case 'L':
				case 'l':
				case 'S':
				case 's':
				case 'P':
				case 'p':
				case 'E':
				case 'e':
					inputOkay = true;
					break;
			}
		} while (!inputOkay);
	}
	else
		//if the operation is specified, set the current operation to that operation
		operation = operationSelected;

	if (operation == 'E' || operation == 'e')
		//if exit command is given, exit this function
		return;

	if (operation != 'P' && operation != 'p') {
		//if the operation isn't playing a single song
		if (indexSelected == -1) {
			//if the index of the playlist isn't specified, print the playlist names
			mainList->listPlaylists(-1);
			cout << endl;
			
			//take inputs until the user enters a valid playlist index
			do {
				cout << "Please enter the index of the playlist: ";
				cin >> index;
				flushInputChars();
			} while (index <= 0 || index > mainList->playlist_count);
		}
		else
			//if the index is specified, set the current index to that index
			index = indexSelected;

		//set p to the address of the playlist with that index
		p = mainList->playlistAtIndex(index);
	}
	else
		//if the operation is playing a single song, set p to the address of the SORTED playlist
		p = mainList->head->next;

	//set the traverse node to the head pointer
	temp = p->head;

	if (operation != 'L' && operation != 'l') {
		//if the operation isn't playing a playlist from the start
		//print the current playlist
		cout << endl << "=========== " << p->name << " PLAYLIST ===========" << endl;
		p->playlistListSongs();
		cout << endl;

		//let the user select a song, take inputs until a valid index is given
		do {
			cout << "Please enter the index of the song to play: ";
			cin >> index;
			flushInputChars();
		} while (index <= 0 || index > p->songnumber);

		//set temp to the given song_node's address
		for (int i = 1; i < index; ++i)
			temp = temp->next;
	}

	//start playing
	cout << endl << "PLAY >" << endl;
	//play while the selection isn't exit
	do {
		cout << "playing . . ." << endl;
		cout << temp->data->name << " - " << temp->data->singer << " - " << temp->data->style << endl;
		if (operation == 'P' || operation == 'p') {
			//if the operation is playing a single song, wait for the player to enter a key
			cout << "Press any key to exit.";
			getchar();
			flushInputChars();
			//exit the loop, so that only one song is played
			cout << endl << endl;
			break;
		}

		//take inputs for each song
		inputOkay = false;
		//until a valid input is given, print the options and ask again
		do {
			cout << "NEXT (N) - PREVIOUS (P) - EXIT (E)" << endl;
			ch = getchar();
			flushInputChars();
			switch (ch) {
				case 'N':
				case 'n':
				case 'P':
				case 'p':
				case 'E':
				case 'e':
					inputOkay = true;
					break;
			}
		} while (!inputOkay);

		if (ch == 'N' || ch == 'n')
			//if next song is selected, move traverse node to the next song
			temp = temp->next;
		else if (ch == 'P' || ch == 'p')
			//if previous song is selected, move traverse node to the previous song
			temp = temp->prev;

	} while (ch != 'E' && ch != 'e');
}

void printList(list *mainList) {
	//initialize index as 1
	int index = 1;
	//set p pointer to the address of CHRONOLOGICAL playlist
	playlist *p = mainList->head;
	char ch, ch2;
	bool inputOkay;

	//do while the 1st menu selection isn't exit
	do {
		//print the 1st menu, take inputs until a valid input is given
		inputOkay = false;
		do {
			//print the playlists and the playlist at the index once more
			mainList->listPlaylists(index);
			//give the user the following options to operate on the list
			cout << "NEXT (N) - PREVIOUS (P) - SELECT (S) - EXIT (E)" << endl;
			cout << "Your choice (N,P,S,E): ";
			ch = getchar();
			flushInputChars();
			switch (ch) {
				case 'N':
				case 'n':
				case 'P':
				case 'p':
				case 'S':
				case 's':
				case 'E':
				case 'e':
					inputOkay = true;
					break;
			}
		} while (!inputOkay);

		switch (ch) {
			case 'N':
			case 'n':
				//if the next option is selected
				if (index == mainList->playlist_count)
					//if index is the last index, turn it back to the first one
					index = 1;
				else
					//if not, increment index
					++index;
				//take the pointer to the next playlist
				p = p->next;
				break;
			case 'P':
			case 'p':
				//if the previous option is selected
				if (index == 1)
					//if index is the first index, turn it back to the last one
					index = mainList->playlist_count;
				else
					//if not, decrement index
					--index;
				//take the pointer to the previous playlist
				p = p->prev;
				break;
			case 'S':
			case 's':
				//if the select option is selected
				if (strcmp(p->name, "RANDOM") == 0)
					//if RANDOM playlist is selected, shuffle its nodes
					mainList->refillRandomPlaylist();
				
				//print the 2nd menu, take inputs until a valid input is given
				inputOkay = false;
				do {
					//print all the songs in the selected playlist
					cout << endl << "=========== " << p->name << " PLAYLIST ===========" << endl;
					p->playlistListSongs();
					//give the user the following options to operate on the playlist
					cout << endl << "DELETE PLAYLIST (D) - ADD SONG (A) - REMOVE SONG (R) - PLAY FROM THE START (P) - EXIT (E)" << endl;
					cout << "Your choice (D,A,R,P,E): ";
					ch2 = getchar();
					flushInputChars();
					switch (ch2) {
						case 'D':
						case 'd':
						case 'A':
						case 'a':
						case 'R':
						case 'r':
						case 'P':
						case 'p':
						case 'E':
						case 'e':
							inputOkay = true;
							break;
					}
				} while (!inputOkay);

				switch (ch2) {
					case 'D':
					case 'd':
						//if the delete option is selected
						//delete this playlist
						deletePlaylist(mainList, index);
						//go back to the CHRONOLOGICAL playlist.
						index = 1;
						p = mainList->head;
						break;
					case 'A':
					case 'a':
						//if add option is selected
						//add a song to this playlist
						addSong(mainList, index);
						break;
					case 'R':
					case 'r':
						//if remove option is selected
						//remove a song from this playlist
						removeSong(mainList, index);
						break;
					case 'P':
					case 'p':
						//if play option is selected
						//play the songs in this playlist from the start
						play(mainList, index, 'L');
						break;
					default:
						//if exit option is selected
						//exit the 2nd menu
						break;
				}

				break;
			case 'E':
			case 'e':
				//if exit option is selected, print a newline and exit the 1st menu
				cout << endl;
				break;
		}

	} while (ch != 'E' && ch != 'e');
}

void addSong(list *mainList, int indexSelected) {
	int index;
	playlist *p;
	song_node *temp, *toAdd;

	if (indexSelected == -1) {
		//if a playlist index isn't specified
		//print all playlists
		mainList->listPlaylists(-1);
		cout << endl;

		//let the user choose one, take inputs until a valid index has been entered
		do {
			cout << "Please enter the index of the playlist that you want to add a song to: ";
			cin >> index;
			flushInputChars();
		} while (index <= 0 || index > mainList->playlist_count);
	}
	else
		//if a playlist index is specified, set the index to that index
		index = indexSelected;

	//set p to the address that points to the playlist at the given index
	p = mainList->playlistAtIndex(index);

	//print the songs inside the SORTED playlist
	cout << endl << "=========== SORTED PLAYLIST ===========" << endl;
	mainList->head->next->playlistListSongs();
	cout << endl;

	//let the user choose a song, take inputs until a valid index has been entered
	do {
		cout << "Please enter the index of the song that you want to add to the selected playlist: ";
		cin >> index;
		flushInputChars();
	} while (index <= 0 || index > mainList->head->next->songnumber);

	//set traverse pointer to the head of the SORTED playlist
	temp = mainList->head->next->head;

	//traverse until the desired song is found
	for (int i = 1; i < index; ++i)
		temp = temp->next;

	//allocate dynamic memory to an unlinked song_node and add the given song
	toAdd = new song_node;
	toAdd->addSongNodeData(temp->data);
	if (p->playlistAddSongNode(toAdd, true))
		//if the song was added successfully, print a message for the user
		cout << "Added the song to playlist " << p->name << " successfully." << endl << endl;
	else
		//if not, the user tried to edit SORTED, CHRONOLOGICAL or RANDOM playlists, print a message to the user
		cout << "Could not add song, you cannot edit SORTED, CHRONOLOGICAL, or RANDOM playlists." << endl << endl;
}

void removeSong(list *mainList, int indexSelected) {
	int index;
	playlist *p;

	if (indexSelected == -1) {
		//if a playlist index isn't specified
		//print all playlists
		mainList->listPlaylists(-1);
		cout << endl;

		//let the user choose one, take inputs until a valid index has been entered
		do {
			cout << "Please enter the index of the playlist that you want to remove a song from: ";
			cin >> index;
			flushInputChars();
		} while (index <= 0 || index > mainList->playlist_count);
	}
	else
		//if a playlist index is specified, set the index to that index
		index = indexSelected;

	//set p to the address that points to the playlist at the given index
	p = mainList->playlistAtIndex(index);

	//print the songs inside the given playlist
	cout << endl << "=========== " << p->name << " PLAYLIST ===========" << endl;
	p->playlistListSongs();
	cout << endl;

	//let the user choose a song, take inputs until a valid index has been entered
	do {
		cout << "Please enter the index of the song that you want to remove: ";
		cin >> index;
		flushInputChars();
	} while (index <= 0 || index > p->songnumber);

	if (p->playlistRemoveSongNode(index, true))
		//if the song was removed successfully, print a message for the user
		cout << "Successfully removed." << endl << endl;
	else
		//if not, the user tried to edit SORTED, CHRONOLOGICAL or RANDOM playlists, print a message to the user
		cout << "Could not remove song, you cannot edit SORTED, CHRONOLOGICAL, or RANDOM playlists." << endl << endl;
}

void createPlaylist(list *mainList) {
	//define buffer variables
	char ch, *buf1, *buf2;
	bool inputOkay = false;
	int i, indexes[100];

	//print the menu and ask for an option, take inputs until a valid input is entered
	do {
		cout << "Please choose one: " << endl;
		cout << "S: Create a playlist based on the song's style" << endl;
		cout << "C: Create a playlist that is a combination of other playlists" << endl;
		cout << "A: Create a playlist based on the artist" << endl;
		cout << "N: Create a new playlist by selecting individual songs" << endl;
		cout << "E: Exit" << endl << endl;
		cout << "Your choice (S,C,A,N,E): ";
		ch = getchar();
		flushInputChars();

		switch (ch) {
			case 'S':
			case 's':
			case 'C':
			case 'c':
			case 'A':
			case 'a':
			case 'N':
			case 'n':
			case 'E':
			case 'e':
				inputOkay = true;
				break;
		}
	} while (!inputOkay);

	switch (ch) {
		case 'S':
		case 's':
			//if a playlist based on style is to be created
			//get and capitalize the desired style string
			cout << "Please enter a style: ";
			buf1 = new char[STYLE_LENGTH + 1];
			cin.getline(buf1, STYLE_LENGTH);
			capitalize(buf1);

			//get a new name for the new playlist
			cout << "Please enter a name for your new playlist: ";
			buf2 = new char[LNAME_LENGTH + 1];
			cin.getline(buf2, LNAME_LENGTH);


			if (mainList->createStylePlaylist(buf2, buf1))
				//if playlist was created successfully, print a message to the user
				cout << "Playlist " << buf2 << " was created successfully." << endl << endl;
			else
				//if not, a playlist with the same name exists, print a message to the user
				cout << "Could not create playlist, the playlist name already exists." << endl << endl;

			//delete buffer variables
			delete buf1;
			delete buf2;
			break;
		case 'C':
		case 'c':
			//if a playlist that is a combination of other playlists is to be created
			//print all the playlists
			mainList->listPlaylists(-1);
			//ask the user to enter the indexes of the playlists to concatenate followed by a -1
			cout << endl << "Please write the indexes of the playlists you want to concatenate; in order, with spaces, followed by a -1: " << endl;
			//until -1 is seen, take inputs (invalid inputs are ignored), i will be equal to the size of the list of indexes
			i = 0;
			while (true) {
				cin >> indexes[i];
				if (indexes[i] == -1)
					break;
				++i;
			}
			flushInputChars();

			//get a new name for the new playlist
			cout << "Please enter a name for your new playlist: ";
			buf2 = new char[LNAME_LENGTH + 1];
			cin.getline(buf2, LNAME_LENGTH);

			if (mainList->createCombinationPlaylist(buf2, indexes, i))
				//if playlist was created successfully, print a message to the user
				cout << "Playlist " << buf2 << " was created successfully." << endl << endl;
			else
				//if not, a playlist with the same name exists, print a message to the user
				cout << "Could not create playlist, the playlist name already exists." << endl << endl;

			//delete buffer variable
			delete buf2;
			break;
		case 'A':
		case 'a':
			//if a playlist based on the artist is to be created
			//get and capitalize the desired artist name string
			cout << "Please enter an artist: ";
			buf1 = new char[SINGER_LENGTH + 1];
			cin.getline(buf1, SINGER_LENGTH);
			capitalize(buf1);

			//get a new name for the new playlist
			cout << "Please enter a name for your new playlist: ";
			buf2 = new char[LNAME_LENGTH + 1];
			cin.getline(buf2, LNAME_LENGTH);

			if (mainList->createSingerPlaylist(buf2, buf1))
				//if playlist was created successfully, print a message to the user
				cout << "Playlist " << buf2 << " was created successfully." << endl << endl;
			else
				//if not, a playlist with the same name exists, print a message to the user
				cout << "Could not create playlist, the playlist name already exists." << endl << endl;

			//delete buffer variables
			delete buf1;
			delete buf2;
			break;
		case 'N':
		case 'n':
			//if a playlist from the given individual songs is to be created
			//print the songs inside the SORTED playlist
			cout << "=========== SORTED PLAYLIST ===========" << endl;
			mainList->head->next->playlistListSongs();
			//ask the user to enter the indexes of the songs to concatenate followed by a -1
			cout << endl << "Please write the indexes of the songs you want to concatenate; in order, with spaces, followed by a -1: " << endl;
			//until -1 is seen, take inputs (invalid inputs are ignored), i will be equal to the size of the list of indexes
			i = 0;
			while (true) {
				cin >> indexes[i];
				if (indexes[i] == -1)
					break;
				++i;
			}
			flushInputChars();

			//get a new name for the new playlist
			cout << "Please enter a name for your new playlist: ";
			buf2 = new char[LNAME_LENGTH + 1];
			cin.getline(buf2, LNAME_LENGTH);

			if (mainList->createSongSpecifiedPlaylist(buf2, indexes, i))
				//if playlist was created successfully, print a message to the user
				cout << "Playlist " << buf2 << " was created successfully." << endl << endl;
			else
				//if not, a playlist with the same name exists, print a message to the user
				cout << "Could not create playlist, the playlist name already exists." << endl << endl;

			//delete buffer variable
			delete buf2;
			break;
		default:
			//if exit option is selected, print a newline and exit the menu
			cout << endl;
			break;
	}
}

void deletePlaylist(list *mainList, int indexSelected) {
	int index;

	if (indexSelected == -1) {
		//if a playlist index isn't specified
		//print all playlists
		mainList->listPlaylists(-1);
		cout << endl;

		//let the user choose one, take inputs until a valid index has been entered
		do {
			cout << "Please enter the index of the playlist to delete: ";
			cin >> index;
		} while (index <= 0 || index > mainList->playlist_count);
	}
	else
		//if a playlist index is specified, set the index to that index
		index = indexSelected;

	if (mainList->deletePlaylist(index, true))
		//if the playlist was deleted successfully, print a message to the user
		cout << "Successfully deleted." << endl << endl;
	else
		//if the playlist could not be deleted, the user is trying to delete the SORTED, CHRONOLOGICAL or the RANDOM playlist, print a message to the user
		cout << "Could not delete playlist, you cannot delete SORTED, CHRONOLOGICAL, or RANDOM playlists." << endl << endl;
}

void writeToFile(list *mainList) {
	//write the list content onto songbook.txt
	mainList->listWriteFile();
	//print a message to the user
	cout << "Saved to file." << endl << endl;
}

void exit(list *mainList) {
	//deallocate everything in the list
	mainList->closeList();
}

void printMainMenu() {
	//print the main menu
	cout << "SONG BOX APPLICATION:" << endl;
	cout << "Choose an operation" << endl;
	cout << "P: Play" << endl;
	cout << "L: Show all playlists" << endl;
	cout << "A: Add songs to a playlist" << endl;
	cout << "R: Remove songs from a playlist" << endl;
	cout << "C: Create a new playlist" << endl;
	cout << "D: Delete a playlist" << endl;
	cout << "W: Write to file (SAVE)" << endl;
	cout << "E: Exit" << endl << endl;
	cout << "Enter a choice (P,L,A,R,C,D,W,E): ";
}

bool makeDecision(list *mainList, char choice) {
	switch (choice) {
		case 'P':
		case 'p':
			//if play option is selected
			//ask the user how and which songs to play, and play them
			play(mainList, -1, '\0');
			return true;
		case 'L':
		case 'l':
			//if the list playlists option is selected
			//list the playlists and let the user choose one, and then let the user operate on that playlist
			printList(mainList);
			return true;
		case 'A':
		case 'a':
			//if the add song option is selected
			//ask the user which playlist to add a song to, and which song to add, and add it to the playlist
			addSong(mainList, -1);
			return true;
		case 'R':
		case 'r':
			//if the remove song option is selected
			//ask the user which playlist to remove a song from, and which song to remove, and remove it from the playlist
			removeSong(mainList, -1);
			return true;
		case 'C':
		case 'c':
			//if the create playlist option is selected
			//ask the user how to create the playlist, and its name, and create the playlist
			createPlaylist(mainList);
			return true;
		case 'D':
		case 'd':
			//if the delete playlist option is selected
			//ask the user which playlist to delete, and delete that playlist
			deletePlaylist(mainList, -1);
			return true;
		case 'W':
		case 'w':
			//if the write to file option is selected
			//write list content onto songbook.txt
			writeToFile(mainList);
			return true;
		case 'E':
		case 'e':
			//if the exit option is selected
			//deallocate everything in list
			exit(mainList);
			//give the exit call
			return false;
		default:
			//if the option is invalid, warn the user
			cout << "Invalid Choice. Try again." << endl << endl;
			return true;
	}
}

void flushInputChars() {
	//this function is used in removing the newlines and extra characters left in the input stream by getchar() and cin >>
	while (getchar() == EOF);
	//calls getchar() until it returns EOF, which is returned whenever a character cannot be read
	//this causes the leftover characters to be ignored
}