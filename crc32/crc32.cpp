#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <cstring>
#include <cstdlib>

#include <sys/stat.h>

#include "crc32.h"

using namespace std;

CRC32::CRC32(char *fileName, bool verbose) {
    _verbose = verbose;
    _fileName = new char[strlen(fileName)+1];
    strncpy(_fileName, fileName, strlen(fileName));
    _fileName[strlen(fileName)] = 0;
    
    //computeChecksum();
}

CRC32::~CRC32() {
    delete [] _fileName;
}

int CRC32::computeChecksum() {
    _crc32 = 0xffffffff;
    unsigned int bufferSize = BUFFERSIZE;
    unsigned char *buffer;
    unsigned int done_pct = 0.0, done_pct_old = 0.0;
    struct stat fileStats;
    unsigned long int taille, reste;

    _file.open(_fileName, ios::in|ios::binary);
    
    if (_file.fail()) {
        cerr << _fileName << ": Doesn't exist" << endl;
        _file.close();
        return -1;
    }
    
    stat(_fileName, &fileStats);
    if (S_ISDIR(fileStats.st_mode)) {
        cerr << _fileName << ": Is a directory" << endl;
        _file.close();
        return -1;
    }

    _file.seekg(0, ios_base::end); //avancer de 0 a partir de la fin
    taille = _file.tellg(); //taille
    _file.seekg(0, ios_base::beg); //avancer de 0 a partir du debut
    reste = taille;
    
    buffer = new unsigned char[bufferSize];

    if (bufferSize > reste)
        bufferSize = reste;

    while (reste) {
        _file.read((char*)buffer, bufferSize);

        for (unsigned int i=0; i<bufferSize; i++) {
            _crc32 = (_crc32 >> 8) ^ crcTable[(_crc32 ^ buffer[i]) & 0xFF];
        }

        reste -= bufferSize;

        if (bufferSize > reste)
            bufferSize = reste;
        
        /*
        Affiche l'avancement du calcul
            "xxx%"      4 chars
            " -> "      4 chars
            "[ nnn]+"   n chars
            " of "      4 chars
            '[ nnn]+"   n chars
        */
        if (_verbose) {
            done_pct = ((double)(taille-reste)/(double)taille)*1000; //Donne au 1/1000
            char s_taille[48];
            strcpy(s_taille, separer_milliers(taille));
            
            if (done_pct != done_pct_old) {
                /*cerr << setw(5) << done_pct/10.0 << "% -> " << setw(strlen(s_taille)) << separer_milliers(taille-reste)
                    << " sur " << s_taille << '\r';*/
                cerr << _fileName << ": " << done_pct/10 << '.' << done_pct%10 << "%\r" << flush;
                done_pct_old = done_pct;
            }
        }
    }
    
    //Pour s'assurer d'un retour a la ligne apres la boucle si _verbose
    /*if (_verbose)
        cerr << endl;*/
    
    _file.close();
    delete [] buffer;

    _crc32 = _crc32 ^ 0xffffffff;
    
    return 0;
}

unsigned int CRC32::getChecksum() {
    return _crc32;
}

char* CRC32::getFileName() {
    return _fileName;
}

const char* CRC32::separer_milliers(unsigned long int nbre) {
    static char buffer[48]; //assez pour 10^36-1
    char *p = buffer + sizeof buffer;
    
    int n = nbre, i = 0;
    
    //On commence par la fin (merci wget!)
    *--p = '\0';
    
    while(1) {
        *--p = n%10 + '0';
        n /= 10;
        
        if (n == 0)
            break;
        
        if (++i%3 == 0)
            *--p = ' ';
    }
    
    return p;
}
    
void CRC32::printChecksum() {
    cout << getFileName() << " " << hex << setw(8) << setfill('0') << uppercase << _crc32 << endl;
}
