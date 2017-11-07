/* @Author
 * Student Name: Oguz Paksoy
 * Student ID: 150150111
 * Date: 09.11.2016 */
#define SNAME_LENGTH 50
#define LNAME_LENGTH 50
#define STYLE_LENGTH 30
#define SINGER_LENGTH 50

#define SONG_COUNT 23

struct playlist;

struct song {
  char* name;
  char* singer;
  char* style;
  int year;

  //inserts data into the song structure
  void addSongData(char *songName, char *songSinger, char *songStyle, int songYear);
  //deallocates data in the song structure
  void deallocateSongData(); 
};

struct song_node {
  song* data;
  song_node* next;
  song_node* prev;
  playlist* parent;

  //initiates song_node structure, links its data to a song pointer
  void addSongNodeData(song *newSong); 
  //deallocates song data and the song structure
  void deallocateSongNodeData(); 
};

struct playlist {
  int songnumber;
  char* name;
  song_node* head;
  playlist* next;
  playlist* prev;

  //initializes a playlist and adds name data
  void playlistAddData(char *pl_name); 
  //adds a song_node to a playlist, unless the playlist is SORTED, CHRONOLOGICAL, or RANDOM when called by the user
  bool playlistAddSongNode(song_node *newNode, bool userCall); 
  //removes a song_node from a playlist at the given index, unless the playlist is SORTED, CHRONOLOGICAL, or RANDOM when called by the user
  bool playlistRemoveSongNode(int index, bool userCall); 
  /* removes all nodes and deallocates playlist data unless the playlist is SORTED, CHRONOLOGICAL, or RANDOM when called by the user
   * if the playlist is RANDOM, it also deallocates song data */
  bool playlistRemoveData(bool userCall); 
  //lists all songs in the playlist
  void playlistListSongs(); 
};

struct list {
  int playlist_count;
  playlist* head;

  /* initializes list, SORTED, CHRONOLOGICAL, RANDOM playlists and the song structures, starts srand()
   * also calls listReadFile() */
  void initializeList(); 
  //returns a song that is found in the SORTED playlist
  song* searchSorted(char *n, char *s, char *st, int yr);
  //returns the playlist at the given index
  playlist* playlistAtIndex(int index);
  //creates a playlist with the given name, and puts it onto the doubly linked circular list
  bool createPlaylist(playlist **p, char *pl_name);
  //fills the CHRONOLOGICAL playlist with the song_nodes that are sorted chronologically
  void fillChronologicalPlaylist(song **songs); 
  //fills the SORTED playlist with the song_nodes that are sorted alphabetically, first by singer name, then by song name
  void fillSortedPlaylist(song **songs); 
  //fills the RANDOM list for the first time
  void fillRandomPlaylist(); 
  //refills the RANDOM list at each selection
  void refillRandomPlaylist(); 
  //creates and fills a playlist based on the given style
  bool createStylePlaylist(char *pl_name, char *Style);
  //creates and fills a playlist based on the given artist
  bool createSingerPlaylist(char *pl_name, char *Singer);
  //creates and fills a playlist with other playlists' contents, represented by their indexes
  bool createCombinationPlaylist(char *pl_name, int *indexes, int size);
  //creates and fills a playlist with specific songs from the SORTED playlist, represented by their indexes
  bool createSongSpecifiedPlaylist(char *pl_name, int *indexes, int size);
  //deletes the playlist at index, unless the playlist is SORTED, CHRONOLOGICAL, or RANDOM when called by the user
  bool deletePlaylist(int index, bool userCall);
  //reads from file, creates and fills all the playlists
  void listReadFile(song **songs); 
  //writes the CHRONOLOGICAL playlist and the user created playlists to a songbook.txt
  void listWriteFile(); 
  /* lists playlists and prints out the name of the playlist at the index once more
   * if given index is -1, the function doesn't print out an extra playlist name */
  void listPlaylists(int index);
  /* deallocates everything in the list
   * the only way SORTED, CHRONOLOGICAL, and RANDOM playlists and the song data are deallocated in the program
   * also calls listWriteFile */
  void closeList(); 
};
/* comparison function, tells the fill functions whether to swap two nodes or not
 * alphabetical mode swaps are used to sort the SORTED playlist data 
 * chronological mode swaps are used to sort the CHRONOLOGICAL playlist data */
bool songNodeCompare(int code, song_node *sn1, song_node *sn2);
//wraps all song data in song_nodes, returns the adress of where the song_node adresses are kept
song_node** toSongNodes(song **songs);
//makes a string uppercase
void capitalize(char *str); 