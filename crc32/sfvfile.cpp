#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

#include <linux/limits.h>
#include <sys/stat.h>

#include "crc32.h"
#include "sfvfile.h"

using namespace std;

SfvFile::SfvFile(char *file) {
    _sfvFile = new char[strlen(file)+1];
    strncpy(_sfvFile, file, strlen(file));
}

SfvFile::~SfvFile() {
    delete [] _sfvFile;
}

void SfvFile::checkSfvFile(bool presence) {
    int nbErr = 0;
    fstream sfvFile;
    char buf[PATH_MAX+256]; //enough for name and checksum
    char fileName[PATH_MAX];
    unsigned int checksum;
    struct stat fileStats;
    
    sfvFile.open(_sfvFile, ios_base::in);
    if (sfvFile.fail()) {
        cerr << _sfvFile << ": Doesn't exist" << endl;
        sfvFile.close();
        exit(EXIT_FAILURE);
    }
    
    stat(_sfvFile, &fileStats);
    if (S_ISDIR(fileStats.st_mode)) {
        cerr << _sfvFile << ": Is a directory" << endl;
        sfvFile.close();
        exit(EXIT_FAILURE);
    }
    
    printHeader();
    
    while (sfvFile.getline(buf, sizeof(buf))) {
        if (buf[0] == ';')
            continue;
        sscanf(buf, "%s %x", fileName, &checksum);
        
        //Verification du fichier
        fstream tmp;
        tmp.open(fileName);
        if (tmp.fail()) {
            cerr << "ERR: "; perror(fileName);
            nbErr++;
            tmp.close();
            continue;
        } else {
            tmp.close();
        }
        
        if (presence == true) {
            cout << "EX : " << fileName << endl;
            continue;
        }
        
        CRC32 crc(fileName);
        crc.computeChecksum();
        if (crc.getChecksum() == checksum) {
            cout << "OK : " << fileName << endl;
        } else if (crc.getChecksum() > 0) {
            cout << "BAD: " << fileName << endl;
            nbErr++;
        }
    }
    
    sfvFile.close();
    
    printFooter();
    
    if (nbErr == 0)
        cout << "Tout est OK!" << endl;
    else if (nbErr == 1)
        cout << "Il y a eu 1 erreur" << endl;
    else
        cout << "Il y a eu " << nbErr << " erreurs" << endl;
}

void SfvFile::printHeader() {
    char buffer[82], *p;
    
    string str = "Verification de ";
    str += _sfvFile;
    
    if (str.length() > 76) {
        str = str.substr(0, 73);
        str += "...";
    }
    
    memset(buffer, '-', 80);
    
    //On se positionne dans le buffer
    //Et on remplace les '-' du centre
    p = buffer + (80 - str.length())/2;
    strncpy(p, str.c_str(), str.length());
    
    buffer[80] = '\n';
    buffer[81] = '\0';
    
    cout << buffer;
}

void SfvFile::printFooter() {
    char buffer[82];
    
    memset(buffer, '-', 80);
    
    buffer[80] = '\n';
    buffer[81] = '\0';
    
    cout << buffer;
}
