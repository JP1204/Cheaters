#include <iostream>
#include <string>
#include <vector>
#define MAX_ROWS 27
#define MAX_COL 27

using namespace std;


struct ChunkNode{
    int fileIndex;
    ChunkNode *next;
};

struct FileNode{
    int fileNum1;	// contains the 2 indices of the files
    int fileNum2;
    int collisionCount;

    FileNode *next;
};

class HashTable{
    public:
	HashTable();
        int hash(vector<string> chunk);		// hashing function to map onto an entry Index
	void insert(int fileIdx, int entryIndex);	// adds a node to the LL with the file index
        void countSim(int entryIndex);		// counts elements in LL at entryIndex

	void getEntry(int entryNum);
	int getTableSize()
	    { return TABLE_SIZE; }

	int getMatchCount(int i, int j)
            { return matchCount[i][j]; }
       
        void printCollisions(vector<string> files);		// traverses LL and prints collision bw every 2 files

        ~HashTable();	

    private:
	static const int TABLE_SIZE = 100003;

        ChunkNode *table[TABLE_SIZE];

	int matchCount[MAX_ROWS][MAX_COL];

        FileNode *head;		// linked list	
};
