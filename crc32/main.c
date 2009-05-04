#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <crc32.h>

int main(int argc, char **argv) {
    uint32_t crc32;
    
	if (argc != 2) {
		fprintf(stderr, "%s: No input file\n", argv[0]);
		exit (EXIT_FAILURE);
	}
    
    if (crc32_hash_file(argv[1], &crc32) != 0)
		exit (EXIT_FAILURE);
    
	printf("%s: %.8x\n", argv[1], crc32);
	
	return EXIT_SUCCESS;
}
