#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "hash.h"
#include "cheaters.h"

using namespace std;


int main(int argc, char *argv[])
{
    // retrieve all the files in the directory
    if(argv[1] == NULL){
        cout << "You forgot the directory!" << endl;
	return -1;
    }

    string dir = string(argv[1]);
    vector<string> files = vector<string>();	// contains all the file names in the directory

    getdir(dir,files);

    for (unsigned int i = 0;i < files.size();i++) {
        cout << i << files[i] << endl;
    }

    // initialize the hash table
    HashTable hashtable;

    // gets chunk size for checking
    if(argv[2] == NULL){
        cout << "Missing chunk size" << endl;
	return -1;
    }

    int chunkSize = stoi(argv[2]);
cout << "chunk size is " << chunkSize << endl;

    string words;
    vector<string> chunk;
    

    // opens every doc in the directory
    for(int i = 0; i < files.size(); i++){
        ifstream doc(argv[1] + files[i]);
	chunk.clear();	// clears vector for each new doc
        
        if(doc.is_open()){
           //  chunk.clear();	// clears the vector to get new chunks in new document
            cout << "*****************************" << endl;

	    for(int i = 0; i < chunkSize; i++){
	        doc >> words;
	        chunk.push_back(words);
	    }
            printChunk(chunk);

	    int entryNum = hashtable.hash(chunk);	// hashes string onto an entryNum
	    hashtable.insert(i, entryNum);		// inserts fileNum into table entry

            while(doc >> words){		// continue for the rest of the doc
 	        chunk.erase(chunk.begin());
                chunk.push_back(words);
//	        printChunk(chunk);

		entryNum = hashtable.hash(chunk);
		hashtable.insert(i, entryNum);
	    }

	    doc.close();
	}
	else{
	    cout << "file " << files[i] << " failed to open" << endl;	
	}
    }

    for(int i = 100000; i < 100002; i++){
        hashtable.getEntry(i);
    }

    for(int i = 0; i < hashtable.getTableSize(); i++){
	  //  cout << "  we are at " << i << endl;
        hashtable.countSim(i);
    }

    for(int i = 0; i < MAX_ROWS; i++){
        for(int j = 0; j < MAX_COL; j++){
            cout << i << ", " << j << " = " << hashtable.getMatchCount(i, j) << endl;
	}
    }

    cout << "here" << endl;


    return 0;
}
