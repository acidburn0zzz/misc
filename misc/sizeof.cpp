#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char *ptr = new char[128];
    printf("Char : %d, Short : %d, Int : %d, Long : %d, Long Int : %d, Long Long : %d, Float : %d, Double : %d, Long Double : %d, Array ptr : %d\n",
        sizeof(char), sizeof(short), sizeof(int), sizeof(long), sizeof(long int),
	   sizeof(long long), sizeof(float), sizeof(double), sizeof(long double), sizeof (ptr));
    delete [] ptr;
    return EXIT_SUCCESS;
}
