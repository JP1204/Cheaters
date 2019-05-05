#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;


/*function... might want it in some class?*/
int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {	// .c_str() switches it to c style string for fxn
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}


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


    // gets chunk size for checking
    if(argv[2] == NULL){
        cout << "Invalid chunk size" << endl;
	return -1;
    }

    int chunkSize = stoi(argv[2]);
cout << "chunk size is " << chunkSize << endl;


    // open one file and prints each chunk
    ifstream firstDoc(argv[1] + files[0]);	// creates stream with directory/filename/
    string words;
    vector<string> chunk;
    
    if(firstDoc.is_open()){

	// gets a string with chunkSize number of words and prints the first chunk
        for(int i = 0; i < chunkSize; i++){
	    firstDoc >> words;
	    chunk.push_back(words); 
	}
        for(auto word = chunk.begin(); word != chunk.end(); word++){
	    cout << *word << " ";
	}
	cout << endl;

	// repeat for the rest of the chunks
	while(firstDoc >> words){
	    chunk.erase(chunk.begin());		// erases the first element
	    chunk.push_back(words);		// and pushes the new word to create new chunk

	    for(auto word = chunk.begin(); word != chunk.end(); word++){
	        cout << *word << " ";
	    }
	    cout << endl;
	}

	firstDoc.close();
    }
    else{
        cout << "file " << files[1] << " failed to open" << endl;
    }


    // repeats for every doc in the directory
    for(int i = 1; i < files.size(); i++){
        ifstream doc(argv[1] + files[i]);
        
        if(doc.is_open()){
            chunk.clear();	// clears the vector to get new chunks in new document
            cout << "*****************************" << endl << endl;

	    for(int i = 0; i < chunkSize; i++){
	        doc >> words;
	        chunk.push_back(words);
	    }
            for(int i = 0; i < chunkSize; i++){
                cout << chunk[i] << " ";
            }
            cout << endl;

            while(doc >> words){
 	        chunk.erase(chunk.begin());
                chunk.push_back(words);
		
		for(int i = 0; i < chunkSize; i++){
		    cout << chunk[i] << " ";
    	 	}
		cout << endl;
	    }

	    doc.close();
	}
	else{
	    cout << "file " << files[i] << " failed to open" << endl;	
	}
    }

	 
    return 0;
}
