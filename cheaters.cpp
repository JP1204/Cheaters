#include <vector>
#include <string>
#include <iostream>
#include <dirent.h>
#include <errno.h>

using namespace std;


void printChunk(vector<string> chunk){
    for(int i = 0; i < chunk.size(); i++){
        cout << chunk[i] << " ";
    }
    cout << endl;
}


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
