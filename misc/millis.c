#include <stdio.h>
#include <sys/time.h>

int get_interval_in_ms(struct timeval a, struct timeval b) {
    return (b.tv_sec - a.tv_sec) * 1000 + (b.tv_usec - a.tv_usec) / 1000;
}

int main() {
    struct timeval tp;
    struct timeval tp2;

    gettimeofday(&tp, NULL);
    printf("Hello, World!!!\n");
    gettimeofday(&tp2, NULL);

    printf("%dms.", get_interval_in_ms(tp, tp2));
    return 0;
}
