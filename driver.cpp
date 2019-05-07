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
    if(files.size() > MAX_ROWS){
        cout << "2D array not large enough for amount of documents" << endl;
        return -1;
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
    // gets threshold value for plagiarism checking
    if(argv[3] == NULL){
        cout << "you forgot the threshold" << endl;
	return -1;
    }


    string words;
    vector<string> chunk;
    
    string directory(argv[1]);
    if(directory[directory.length() - 1] != '/'){
        directory = directory + '/';
    }	

    // opens every doc in the directory
    for(int i = 0; i < files.size(); i++){
        ifstream doc(directory + files[i]);
	chunk.clear();	// clears vector for each new doc
        
        if(doc.is_open()){
           //  chunk.clear();	// clears the vector to get new chunks in new document
            cout << "*****************************" << endl;

	    for(int i = 0; i < chunkSize; i++){
	        doc >> words;
	        chunk.push_back(words);
	    }
//            printChunk(chunk);

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


    // store to the 2D array the number of collisions bw files
    for(int i = 0; i < hashtable.getTableSize(); i++){
        hashtable.countSim(i);
    }

    for(int i = 0; i < MAX_ROWS; i++){
        for(int j = 0; j < MAX_COL; j++){
            cout << i << ", " << j << " = " << hashtable.getMatchCount(i, j) << endl;
	}
    }


    // Displays the results with collisions above the threshold
    int plagThreshold = atoi(argv[3]);
    
    for(int i = 0; i < MAX_ROWS; i++){
        for(int j = (i + 1); j < MAX_COL; j++){
            hashtable.insertFileNode(i, j);
	}
    }

    // sorts the LL from most collisions to least
    hashtable.sortCollisions();

    // print the files that have more collisions than the threshold
    hashtable.printCollisions(files, plagThreshold);

    return 0;
}
