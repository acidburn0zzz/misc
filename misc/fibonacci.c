#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double count = 0;

long int fibonacci(int n) {
    count++;
    return (n <= 1)? 1 : fibonacci(n-1) + fibonacci(n-2);
}

int main(int argc, char *argv[]) {
    long int n;
    int r, t;
    
    if (argc == 2) {
        sscanf(argv[1], "%li", &n);
    } else {
        printf("x = ");
        scanf("%li", &n);
    }
    
    t = time(NULL);
    r = fibonacci(n);
    t = time(NULL) - t;
    
    printf("fibonacci(%li) = %i\n%.0f calculs\nTook %i seconds\n", n, r, count, t);
    return EXIT_SUCCESS;
}
