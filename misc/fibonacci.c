#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

long int count = 0;

long int fibonacci(int n) {
    count++;
    return (n <= 1)? n : fibonacci(n-1) + fibonacci(n-2);
}

int get_interval_in_ms(struct timeval a, struct timeval b) {
        return (b.tv_sec - a.tv_sec) * 1000 + (b.tv_usec - a.tv_usec) / 1000;
}

int main(int argc, char *argv[]) {
    long int n;
    int r;

    struct timeval t1, t2;

    if (argc == 2) {
        sscanf(argv[1], "%li", &n);
    } else {
        printf("x = ");
        if (scanf("%li", &n) != 1) {
            fprintf(stderr, "Error reading from stdin\n");
            exit(1);
        }
    }

    gettimeofday(&t1, NULL);
    r = fibonacci(n);
    gettimeofday(&t2, NULL);

    printf("fibonacci(%li) = %i\n%li steps\nTook %i ms\n", n, r, count, get_interval_in_ms(t1, t2));
    return EXIT_SUCCESS;
}
