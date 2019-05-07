#include "hash.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;


HashTable::HashTable(){		// constructor initializes all pointers (LL) to NULL
    for(int i = 0; i < TABLE_SIZE; i++){
        table[i] = NULL;
    }

    for(int i = 0; i < MAX_ROWS; i++){
        for(int j = 0; j < MAX_COL; j++){
            matchCount[i][j] = 0;
	}
    }

    head = NULL;
}


int HashTable::hash(vector<string> chunk){
    int hashNum = 0;	// the entry number the fxn hashes to
    int scalar = 1;	 

    /* multiplies each word by a different number
     * 1st word by 1, 2nd word by 4, 3rd word by 9 and so on...
     */
    for(auto it = chunk.begin(); it != chunk.end(); it++){
	int wordTotal = 0;
        string word = *it;

        // get the ascii total for each word
	for(int i = 0; i < word.length(); i++){
	    wordTotal += (int) word[i];
	}
        
	int scalarSquared = scalar*scalar;

        wordTotal *= scalarSquared;
        hashNum += wordTotal;
	
	if(scalar > 6){
            scalar = 0;
	}
	else{
	    scalar += 1;
	}
    }

    hashNum = hashNum % TABLE_SIZE;
    return hashNum;
}


void HashTable::insert(int fileIdx, int entryIndex){
    // check if there is a collision
    if(table[entryIndex] == NULL){
        // create first node of LL
	ChunkNode *temp = new ChunkNode;

	temp->fileIndex = fileIdx;
	temp->next = NULL;

	table[entryIndex] = temp;
    }
    else{
        // adds onto the LL
	ChunkNode *temp = new ChunkNode;
        
        temp->fileIndex = fileIdx;
        temp->next = table[entryIndex];	// new node points to old head node

	table[entryIndex] = temp;		// head node updates	
    }
}    


// counts similarities inside an entry and increments the matchCount array
void HashTable::countSim(int entryIndex){
    // check if the entry is an empty LL
    if(table[entryIndex] == NULL){
        return;	
    }

    int fileNum1;
    int fileNum2;

    for(ChunkNode *p1 = table[entryIndex]; p1 != NULL; p1 = p1->next){
        for(ChunkNode *p2 = p1->next; p2 != NULL; p2 = p2->next){
            fileNum1 = p1->fileIndex;
	    fileNum2 = p2->fileIndex;
	    
            if(fileNum1 != fileNum2){
	    	matchCount[fileNum1][fileNum2] += 1;	// increments at indices of file
	    }
	}
    }
}


void HashTable::getEntry(int entryNum){
    cout << "the entry at " << entryNum << " is ";
    ChunkNode *temp = table[entryNum];

    while(temp != NULL){
        cout << temp->fileIndex << ", ";
	temp = temp->next;
    }
    cout << endl;
}


void HashTable::printCollisions(vector<string> files){
    FileNode *temp = head;

    while(temp != NULL){
        cout << temp->collisionCount << ": ";
	cout << files[temp->fileNum1] << " and ";
	cout << files[temp->fileNum2] << endl;
        
	temp = temp->next;
    }
}


HashTable::~HashTable(){
    // delete all nodes from each hash entry
    for(int i = 0; i < TABLE_SIZE; i++){

	if(table[i] != NULL){
    	    ChunkNode *p = table[i];

            while(p != NULL){
                ChunkNode *prev = p;
                p = p->next;

	        delete prev;
	    }
            
            table[i] = NULL;
	}
    }
}    
