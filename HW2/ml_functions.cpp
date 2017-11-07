/* @Author
 * Student Name: Oguz Paksoy
 * Student ID: 150150111
 * Date: 09.11.2016 */
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include "musiclist.h"

#define ALPHABETICAL_SORT 1
#define CHRONOLOGICAL_SORT 2

using namespace std;

bool songNodeCompare(int code, song_node *sn1, song_node *sn2) {
	if (code == ALPHABETICAL_SORT) {
		//Alphabetical mode
		if (strcmp(sn1->data->singer, sn2->data->singer) > 0)
			//return true if 1st songs singer comes after 2nd songs singer alphabetically
			return true;
		else if (strcmp(sn1->data->singer, sn2->data->singer) == 0) {
			//if the singer names are the same
			if (strcmp(sn1->data->name, sn2->data->name) > 0)
				//return true if 1st songs name comes after 2nd songs name alphabetically
				return true;
		}
	}
	else {
		//Chronological mode
		if (sn1->data->year > sn2->data->year)
			//return true if the 1st song came out after the 2nd song
			return true;
	}

	//return false otherwise
	return false;
}

song_node** toSongNodes(song **songs) {
	//allocate a dynamic space for the song_node pointers
	song_node **song_nodes = new song_node*[SONG_COUNT];
	for (int i = 0; i < SONG_COUNT; ++i) {
		//allocate a dynamic space for each node and add a reference to the songs
		song_nodes[i] = new song_node;
		song_nodes[i]->addSongNodeData(songs[i]);
	}
	//return the adresses
	return song_nodes;
}

void capitalize(char *str) {
	//iterate a char pointer over the string until it's value reaches null, replace all characters with an uppercase version when possible
	for (char *p = str; *p = toupper(*p); ++p);
}

//----------------------------------------------

void song::addSongData(char *songName, char *songSinger, char *songStyle, int songYear) {
	//get dynamic memory for name, singer, and style, and copy the data to this song structure
	name = new char[strlen(songName) + 1];
	singer = new char[strlen(songSinger) + 1];
	style = new char[strlen(songStyle) + 1];

	strcpy(name, songName);
	strcpy(singer, songSinger);
	strcpy(style, songStyle);
	year = songYear;
}

void song::deallocateSongData() {
	//deallocate char pointers in this song structure
	delete[] name;
	delete[] singer;
	delete[] style;
}

//----------------------------------------------

void song_node::addSongNodeData(song *newSong) {
	//add a reference to the given song
	data = newSong;
	//initialize all other pointers as null
	next = NULL;
	prev = NULL;
	parent = NULL;
}

void song_node::deallocateSongNodeData() {
	//deallocate the data inside the song structure, and deallocate the song structure itself
	data->deallocateSongData();
	delete data;
}

//----------------------------------------------

void playlist::playlistAddData(char *pl_name) {
	//get dynamic memory for name, copy the given name onto this adress
	name = new char[strlen(pl_name) + 1];
	strcpy(name, pl_name);
	//initialize head pointer as null, songnumber as 0
	head = NULL;
	songnumber = 0;
}

bool playlist::playlistAddSongNode(song_node *newNode, bool userCall) {
	//checks for the current playlist name, which will be edited
	bool isSorted = (strcmp(next->prev->name, "SORTED") == 0),
		 isChronological = (strcmp(next->prev->name, "CHRONOLOGICAL") == 0),
		 isRandom = (strcmp(next->prev->name, "RANDOM") == 0);
	
	//add parent data
	newNode->parent = next->prev;

	//if the user is trying to edit SORTED, CHRONOLOGICAL or RANDOM playlists, return false
	if (userCall && (isSorted || isChronological || isRandom))
		return false;

	if (head == NULL) {
		//if the playlist is empty, place a node to head
		head = newNode;
		//wrap it around itself to create a doubly linked circular list
		newNode->next = newNode;
		newNode->prev = newNode;
	}
	else {
		//if the playlist isn't empty, insert a node right before head and edit next, prev pointers
		newNode->next = head;
		newNode->prev = head->prev;
		head->prev->next = newNode;
		head->prev = newNode;
	}
	//increment the number of songs
	++songnumber;
	
	//return true if not returned already
	return true;
}

bool playlist::playlistRemoveSongNode(int index, bool userCall) {
	//start a traverse node from the head pointer
	song_node *temp = head;
	//checks for the current playlist name, which will be edited
	bool isSorted = (strcmp(next->prev->name, "SORTED") == 0),
		 isChronological = (strcmp(next->prev->name, "CHRONOLOGICAL") == 0),
		 isRandom = (strcmp(next->prev->name, "RANDOM") == 0);
	
	//if the user is trying to edit SORTED, CHRONOLOGICAL or RANDOM playlists, return false
	if (userCall && (isSorted || isChronological || isRandom))
		return false;
	
	//go to the song_node at the given index
	for (int i = 1; i < index; ++i)
		temp = temp->next;

	if (songnumber == 1)
		//if the node to delete was the last node, make head null
		head = NULL;
	else {
		//if not
		if (temp == head)
			//if the head pointer was to be deleted, the next pointer is the new head
			head = head->next;
		//exclude the node from the linked list
		temp->next->prev = temp->prev;
		temp->prev->next = temp->next;
	}
	if (isSorted)
		//if the edited node is SORTED (non-user call), deallocate the song itself
		temp->deallocateSongNodeData();
	//delete the song_node at the index
	delete temp;
	//decrement the number of songs
	--songnumber;
	
	//return true if not returned already
	return true;
}

bool playlist::playlistRemoveData(bool userCall) {
	bool flag = true;

	while (head) {
		//while head isn't null, remove all nodes in the playlist
		if (!playlistRemoveSongNode(1, userCall)) {
			//if the user tries to edit SORTED, CHRONOLOGICAL, or RANDOM playlists, do not erase anything
			flag = false;
			break;
		}
	}

	if (flag)
		//if there is no violation, delete the name
		delete[] name;

	//return true if okay, false if not
	return flag;
}

void playlist::playlistListSongs() {
	//start a traverse node from the head pointer
	song_node *temp = head;

	for (int i = 1; i < songnumber + 1; ++i) {
		//for each song, print out data and pass to the next song pointer
		cout << i << ". " << temp->data->name << "\t" << temp->data->singer << "\t" << temp->data->year << "\t" << temp->data->style << endl;
		temp = temp->next;
	}
}

//----------------------------------------------

void list::initializeList() {
	playlist *chronological, *sorted, *random;
	//array of song pointers to hold the song data
	song *allSongs[SONG_COUNT];
	
	//get dynamic memory for each playlist, link them all
	chronological = new playlist;
	sorted = new playlist;
	random = new playlist;

	chronological->playlistAddData("CHRONOLOGICAL");
	chronological->next = sorted;
	chronological->prev = random;

	sorted->playlistAddData("SORTED");
	sorted->next = random;
	sorted->prev = chronological;

	random->playlistAddData("RANDOM");
	random->next = chronological;
	random->prev = sorted;
	
	//head pointer always points to CHRONOLOGICAL playlist
	head = chronological;
	//current playlist count is 3
	playlist_count = 3;

	//get dynamic memory for each song
	for (int i = 0; i < SONG_COUNT; ++i)
		allSongs[i] = new song;

	//start randomizer
	srand(time(NULL));
	//read from the file to fill the songs and the playlists
	listReadFile(allSongs);
}

song* list::searchSorted(char *n, char *s, char *st, int yr) {
	//start a traverse node from the head pointer of the SORTED playlist
	song_node *temp = head->next->head;
	for (int i = 1; i < SONG_COUNT + 1; ++i) {
		bool singerEq = (strcmp(temp->data->singer, s) == 0),
			 nameEq = (strcmp(temp->data->name, n) == 0),
			 styleEq = (strcmp(temp->data->style, st) == 0),
			 yearEq = (temp->data->year == yr);
		
		//if everything is equal, then the song is found, return its adress
		if (singerEq && nameEq && styleEq && yearEq)
			return temp->data;

		//otherwise keep looking
		temp = temp->next;
	}
	//good practice, never actually returns null
	return NULL;
}

playlist* list::playlistAtIndex(int index) {
	//start a traverse playlist node from the head pointer
	playlist *p = head;

	//go to the playlist in the given index
	for (int i = 1; i < index; ++i)
		p = p->next;

	//return the address of this playlist
	return p;
}

void list::fillSortedPlaylist(song **songs) {
	//set p to the adress of SORTED playlist
	playlist *p = head->next;
	//define a holder for the returned song_node pointer adresses, also dynamically allocate memory for temp
	song_node **song_nodes,
			  *temp = new song_node;

	//put the songs in unlinked song_nodes
	song_nodes = toSongNodes(songs);

	//bubblesort
	for (int i = 0; i < SONG_COUNT - 1; ++i) {
		for (int j = 0; j < SONG_COUNT - i - 1; ++j) {
			if (songNodeCompare(ALPHABETICAL_SORT, song_nodes[j], song_nodes[j + 1])) {
				//nodes are swapped by copying struct contents
				*temp = *song_nodes[j];
				*song_nodes[j] = *song_nodes[j + 1];
				*song_nodes[j + 1] = *temp;
			}
		}
	}
	//delete temp, no longer useful
	delete temp;

	//add these sorted nodes into the SORTED playlist
	for (int i = 0; i < SONG_COUNT; ++i)
		p->playlistAddSongNode(song_nodes[i], false);

	//delete the memory space dynamically allocated by toSongNodes()
	delete song_nodes;
}

void list::fillChronologicalPlaylist(song **songs) {
	//set p to the adress of CHRONOLOGICAL playlist
	playlist *p = head;
	//define a holder for the returned song_node pointer adresses, also dynamically allocate memory for temp
	song_node **song_nodes,
			  *temp = new song_node;

	//put the songs in unlinked song_nodes
	song_nodes = toSongNodes(songs);

	//bubblesort
	for (int i = 0; i < SONG_COUNT - 1; ++i) {
		for (int j = 0; j < SONG_COUNT - i - 1; ++j) {
			if (songNodeCompare(CHRONOLOGICAL_SORT, song_nodes[j], song_nodes[j + 1])) {
				//nodes are swapped by copying struct contents
				*temp = *song_nodes[j];
				*song_nodes[j] = *song_nodes[j + 1];
				*song_nodes[j + 1] = *temp;
			}
		}
	}
	//delete temp, no longer useful
	delete temp;

	//add these sorted nodes into the CHRONOLOGICAL playlist
	for (int i = 0; i < SONG_COUNT; ++i)
		p->playlistAddSongNode(song_nodes[i], false);

	//delete the memory space dynamically allocated by toSongNodes()
	delete song_nodes;
}

void list::fillRandomPlaylist() {
	//set p to the adress of RANDOM playlist
	playlist *p = head->next->next;
	//define a holder for the unlinked song_nodes, set sn to the head of the SORTED playlist
	song_node *song_nodes[SONG_COUNT],
			  *sn = p->prev->head;
	//define integers for keeping track of how many and which places are taken, and an integer for randomplace
	int remaining = SONG_COUNT, 
		randomplace, 
		place[SONG_COUNT] = {0};

	//add data inside SORTED playlist's nodes into dynamically allocated and unlinked song_nodes
	for (int i = 0; i < SONG_COUNT; ++i) {
		song_nodes[i] = new song_node;
		song_nodes[i]->addSongNodeData(sn->data);
		sn = sn->next;
	}

	//for each place
	for (int i = 0; i < SONG_COUNT; ++i) {
		//get a random number between 0 and remaining empty places
		randomplace = rand() % remaining;
		//traverse all places
		for (int j = 0; j < SONG_COUNT; ++j) {
			if (place[j] == 0) {
				//if place is vacant
				if (randomplace == 0) {
					//if random nummber is reduced to zero, this is its place
					//add it to the linked list, mark the place as taken, decrement remaining places and stop looking
					p->playlistAddSongNode(song_nodes[j], false);
					place[j] = 1;
					--remaining;
					break;
				}
				else
					//else decrement random number until it reaches its destination
					--randomplace;
			}
		}
	}	
}

void list::refillRandomPlaylist() {
	//set p to the adress of RANDOM playlist
	playlist *p = head->next->next;
	//remove all data from the RANDOM playlist
	p->playlistRemoveData(false);
	//rewrite its name
	p->playlistAddData("RANDOM");
	//fill the RANDOM playlist again
	fillRandomPlaylist();
}

bool list::createPlaylist(playlist **p, char *pl_name) {
	//set the traverse node to the adress of CHRONOLOGICAL playlist
	playlist *t = head;
	bool flag = true;
	for (int i = 0; i < playlist_count; ++i) {
		if (strcmp(pl_name, t->name) == 0) {
			flag = false;
			break;
		}
	}
	if (!flag)
		//if playlist name already exists, return false
		return false;

	//allocate dynamic memory for *p, capitalize the name and set its name to that
	*p = new playlist;
	capitalize(pl_name);
	(*p)->playlistAddData(pl_name);

	//place the node right before head pointer, link it up
	head->prev->next = *p;
	(*p)->prev = head->prev;
	(*p)->next = head;
	head->prev = *p;
	//increment playlist node counter
	++playlist_count;

	//return true if not returned already
	return true;
}

bool list::createStylePlaylist(char *pl_name, char *Style) {
	//set traverse nodes adress to the first song in SORTED playlist
	song_node *temp = head->next->head;
	playlist *p;
	
	if (!createPlaylist(&p, pl_name))
		//if unable to create playlist, return false
		return false;

	//traverse all songs
	for (int i = 0; i < SONG_COUNT; ++i) {
		if (strcmp(temp->data->style, Style) == 0) {
			//if it matches the style, allocate dynamic memory to an unlinked node and add it to the new playlist
			song_node *toAdd = new song_node;
			toAdd->addSongNodeData(temp->data);
			p->playlistAddSongNode(toAdd, true);
		}
		//otherwise keep looking
		temp = temp->next;
	}

	//return true if not returned already
	return true;
}

bool list::createSingerPlaylist(char *pl_name, char *Singer) {
	//set traverse nodes adress to the first song in SORTED playlist
	song_node *temp = head->next->head;
	playlist *p;
	
	if (!createPlaylist(&p, pl_name))
		//if unable to create playlist, return false
		return false;
	
	//traverse all songs
	for (int i = 0; i < SONG_COUNT; ++i) {
		if (strcmp(temp->data->singer, Singer) == 0) {
			//if it matches the singer, allocate dynamic memory to an unlinked node containing the song address and add it to the new playlist
			song_node *toAdd = new song_node;
			toAdd->addSongNodeData(temp->data);
			p->playlistAddSongNode(toAdd, true);
		}
		//otherwise keep looking
		temp = temp->next;
	}

	//return true if not returned already
	return true;
}

bool list::createCombinationPlaylist(char *pl_name, int *indexes, int size) {
	song_node *temp;
	playlist *p, *t;

	if (!createPlaylist(&p, pl_name))
		//if unable to create playlist, return false
		return false;

	//for each index given by the user
	for (int i = 0; i < size; ++i) {
		if (indexes[i] <= 0 || indexes[i] > playlist_count)
			//if index is invalid, do nothing
			continue;

		//find the desired playlist to add
		t = playlistAtIndex(indexes[i]);

		//set traverse node to the head song_node pointer
		temp = t->head;
		//for each song in the desired playlist
		for (int j = 0; j < t->songnumber; ++j) {
			//allocate dynamic memory to an unlinked node containing the song address and add it to the new playlist
			song_node *toAdd = new song_node;
			toAdd->addSongNodeData(temp->data);
			p->playlistAddSongNode(toAdd, true);
			//continue adding
			temp = temp->next;
		}
	}

	//return true if not returned already
	return true;
}

bool list::createSongSpecifiedPlaylist(char *pl_name, int *indexes, int size) {
	song_node *temp;
	playlist *p, *t;

	if (!createPlaylist(&p, pl_name))
		//if unable to create playlist, return false
		return false;

	//set t to the address of SORTED playlist
	t = head->next;

	//for each index given by the user
	for (int i = 0; i < size; ++i) {
		if (indexes[i] <= 0 || indexes[i] > t->songnumber)
			//if index is invalid, do nothing
			continue;

		//set traverse node to the head song_node pointer
		temp = t->head;
		
		//find desired song in the SORTED playlist
		for (int j = 1; j < indexes[i]; ++j)
			temp = temp->next;

		//allocate dynamic memory to an unlinked node containing the song address and add it to the new playlist
		song_node *toAdd = new song_node;
		toAdd->addSongNodeData(temp->data);
		p->playlistAddSongNode(toAdd, true);
	}

	//return true if not returned already
	return true;
}

bool list::deletePlaylist(int index, bool userCall) {
	//find desired playlist to delete
	playlist *p = playlistAtIndex(index);

	if (!p->playlistRemoveData(userCall))
		//if user wanted to erase SORTED, CHRONOLOGICAL or RANDOM playlists, return false
		return false;

	if (playlist_count == 1)
		//if the playlist to delete was the last playlist, make head null
		head = NULL;
	else {
		//if not
		if (p == head)
			//if the head pointer was to be deleted, the next pointer is the new head
			head = head->next;
		//exclude the playlist from the linked list
		p->prev->next = p->next;
		p->next->prev = p->prev;
	}
	//delete the playlist at the index
	delete p;
	//decrement the number of playlists
	--playlist_count;

	//return true if not returned already
	return true;
}

void list::listPlaylists(int index) {
	//set traverse node p to the address of CHRONOLOGICAL playlist, initialize i as 1
	playlist *p = head, *t;
	int i = 1;


	cout << "PLAYLISTS: " << endl;
	//print playlist name with its index until the traverse node wraps around and becomes the address of CHRONOLOGICAL playlist again
	do {
		cout << i << ". " << p->name << endl;
		++i;
		p = p->next;
	} while (strcmp(p->name, head->name) != 0);

	if (index != -1) {
		//if index is specified, find the specified playlist and print its name once more
		t = playlistAtIndex(index);
		cout << endl << index << ". " << t->name << endl;
	}
}

void list::listReadFile(song **songs) {
	//open songbook.txt for read
	FILE *fr = fopen("songbook.txt", "r");
	//define buffers for everything to be read 
	char buffer[1000],
		 namebuf[SNAME_LENGTH],
		 singerbuf[SINGER_LENGTH],
		 stylebuf[STYLE_LENGTH],
		 pl_namebuf[LNAME_LENGTH];
	int yr;

	//for as long as you don't reach EOF
	for (int i = 0; fgets(buffer, 1000, fr); ++i) {
		if (strncmp(buffer, "*****", 5) == 0)
			//if 5 stars are encountered, gathering of song data is complete
			break;

		//read all values from buffer and store them in the song pointers array
		sscanf(buffer, "%[^\t]\t%[^\t]\t%d\t%[^\r]\r\n", namebuf, singerbuf, &yr, stylebuf);
		songs[i]->addSongData(namebuf, singerbuf, stylebuf, yr);
	}

	//fill SORTED, CHRONOLOGICAL playlist with the given song data
	fillSortedPlaylist(songs);
	fillChronologicalPlaylist(songs);
	//fill RANDOM playlist
	fillRandomPlaylist();
	
	//for as long as you don't reach EOF
	while (fgets(buffer, 1000, fr)) {
		playlist *p;
		//read the name of the custom playlist, create it
		sscanf(buffer, "%[^\r]\r\n", pl_namebuf);
		createPlaylist(&p, pl_namebuf);
		//after that, for as long as you don't reach EOF, keep adding songs
		while (fgets(buffer, 1000, fr)) {
			if (strncmp(buffer, "*****", 5) == 0)
				//if 5 stars are encountered, a new playlist is formed
				break;

			//dynamically allocate data for new nodes
			song_node *temp = new song_node;
			//read all values from buffer
			sscanf(buffer, "%[^\t]\t%[^\t]\t%d\t%[^\r]\r\n", namebuf, singerbuf, &yr, stylebuf);
			//search the SORTED playlist for the songs, and add their data to unlinked song_nodes when found
			temp->addSongNodeData(searchSorted(namebuf, singerbuf, stylebuf, yr));
			//add nodes to the playlist
			p->playlistAddSongNode(temp, false);
		}
	}

	//close songbook.txt
	fclose(fr);
}

void list::listWriteFile() {
	//open songbook.txt for write
	FILE *fw = fopen("songbook.txt", "w");
	//set t to the address of CHRONOLOGICAL playlist
	playlist *t = head;
	//set temp to its head
	song_node *temp = t->head;
	
	//for all songs in CHRONOLOGICAL playlist
	for (int i = 0; i < SONG_COUNT; ++i) {
		//write data onto the file
		fprintf(fw, "%s\t%s\t%d\t%s\r\n", temp->data->name,
										  temp->data->singer,
										  temp->data->year,
										  temp->data->style);
		//pass on to the next node
		temp = temp->next;
	}

	//set t to the first custom playlist's address
	t = t->next->next->next;
	//while the pointer doesn't wrap back to the CHRONOLOGICAL playlist
	while (strcmp(t->name, "CHRONOLOGICAL") != 0) {
		//print 5 stars and the name
		fprintf(fw, "*****\r\n%s\r\n", t->name);
		//set temp pointer to the head
		temp = t->head;
		//for each song
		for (int j = 0; j < t->songnumber; ++j) {
			//write data onto the file
			fprintf(fw, "%s\t%s\t%d\t%s\r\n", temp->data->name,
											  temp->data->singer,
											  temp->data->year,
											  temp->data->style);
			//pass on to the next node
			temp = temp->next;
		}
		//move to the next playlist
		t = t->next;
	}

	//close songbook.txt
	fclose(fw);
}

void list::closeList() {
	//write the playlists and the CHRONOLOGICAL playlist to songbook.txt
	listWriteFile();

	//move head pointer to RANDOM playlist, so that SORTED gets deleted last
	head = head->next->next;

	//delete all playlists and deallocate everything
	while (head)
		deletePlaylist(1, false);
}