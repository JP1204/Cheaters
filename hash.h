#include <iostream>
#include <string>
#include <vector>
#define MAX_ROWS 25
#define MAX_COL 25

using namespace std;


struct ChunkNode{
    int fileIndex;
    ChunkNode *next;
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
        
        ~HashTable();	

    private:
	static const int TABLE_SIZE = 100003;

        ChunkNode *table[TABLE_SIZE];
	int matchCount[MAX_ROWS][MAX_COL];        	

};
