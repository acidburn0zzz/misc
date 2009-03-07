#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>

#include "crc32.h"
#include "sfvfile.h"

#define __PROG_VERSION__ "1.0b1 "

using namespace std;

void usage(char *prog) {
    cout << "AcidCRC32 " << __PROG_VERSION__ << endl;
    cout << "Usage :" << endl;
    cout << "\t" << prog << " [-pv] -c sfv_file" << endl;
    cout << "OR" << endl;
    cout << "\t" << prog << " [-v] file(s)" << endl;
    exit(EXIT_FAILURE);
}

void printHeader() {
    time_t rawtime;
    tm *timeinfo;
    char buffer[128];
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    if (strftime(buffer, 128, "le %x a %X", timeinfo) == 0)
        buffer[0] = 0;
    
    cout << "; Genere par AcidCRC32 " << __PROG_VERSION__ << buffer << endl;
    cout << ";" << endl;
}    

int main(int argc, char *argv[]) {
    char *file = NULL;
    char ch;
    bool presence = false;
    bool verify = false;
    bool verbose = false;
    
    if (argc < 2)
        usage(argv[0]);
    
    while ((ch = getopt(argc, argv, "c:hpv")) != -1)
	switch (ch) {
	case 'c':
        file = new char[strlen(optarg) + 1];
	    strncpy(file, optarg, strlen(optarg));
	    verify = true;
	    break;
	case 'h':
	    usage(argv[0]);
	    break;
	case 'p':
	    presence = true;
	    break;
	case 'v':
	    verbose = true;
	    break;
	default:
	    usage(argv[0]);
        }
    argc -= optind;
    argv += optind;
    
    if (verify) {
        SfvFile sfv(file);
        sfv.checkSfvFile(presence);
        
        delete [] file;
        return EXIT_SUCCESS;
    }
    
    //Si on est ici, on affiche les checksum
    printHeader();
    
    while(argc--) {
        CRC32 crc(*argv++, verbose);
        if (crc.computeChecksum() == 0)
            crc.printChecksum();
    }
    return 0;
}
