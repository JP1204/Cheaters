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


void HashTable::printCollisions(vector<string> files, int threshold){
    FileNode *temp = head;

    while(temp != NULL){
	if(temp->collisionCount > threshold){
            cout << temp->collisionCount << ": ";
	    cout << files[temp->fileNum1] << " and ";
	    cout << files[temp->fileNum2];
	    cout << " (files " << temp->fileNum1 << " and " << temp->fileNum2 << ")" << endl;
	}
	
	temp = temp->next;
    }
}


void HashTable::insertFileNode(int i, int j){
    // creates node for LL that tracks collisions bw files
    FileNode *temp = new FileNode;

    temp->fileNum1 = i;
    temp->fileNum2 = j;

    // gets similarities bw file i and j
    int count1 = this->getMatchCount(i, j);
    int count2 = this->getMatchCount(j, i);
    temp->collisionCount = count1 + count2;
    
    temp->next = head;
    head = temp;
}


void HashTable::sortCollisions(){
    // implement selection sort
    if(head == NULL){
        return;
    }
    
    for(FileNode *p1 = head; p1 != NULL; p1 = p1->next){    
        FileNode *maxCollisions;
	maxCollisions = p1;
        
        for(FileNode *p2 = p1->next; p2 != NULL; p2 = p2->next){ 
	    if(p2->collisionCount > maxCollisions->collisionCount){
	        maxCollisions = p2;
            }
	}

        // swaps the largest element with the first element
	if(p1 != maxCollisions){
	    FileNode temp = {.fileNum1 = p1->fileNum1, .fileNum2 = p1->fileNum2, .collisionCount = p1->collisionCount, .next = NULL};
            
	    p1->fileNum1 = maxCollisions->fileNum1;
	    p1->fileNum2 = maxCollisions->fileNum2;
	    p1->collisionCount = maxCollisions->collisionCount;

	    maxCollisions->fileNum1 = temp.fileNum1;
	    maxCollisions->fileNum2 = temp.fileNum2;
	    maxCollisions->collisionCount = temp.collisionCount;
	}
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

    for(FileNode *temp = head; temp != NULL; temp = temp->next){
        FileNode *prev = temp;
        delete prev;
    }

    head = NULL;    
}    
