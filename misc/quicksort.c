#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <sys/time.h>

long cnt;

void init(int *arr, int size);
long get_elapsed_ms(struct timeval t1, struct timeval t2);
void swap(int *a, int *b);
void quicksort(int *arr, int left, int right);
void quicksort2(int *arr, int l, int u);
void bubble(int *arr, int size);
void flat(int *arr, int size);

int main(int argc, char **argv) {
    int *aa, *ab, *ac, *ad, i;
    struct timeval t1, t2;
    const int ARRAY_SIZE = 100000;

    srand(time(0));
    aa = malloc(sizeof(int) * ARRAY_SIZE);
    ab = malloc(sizeof(int) * ARRAY_SIZE);
    ac = malloc(sizeof(int) * ARRAY_SIZE);
    ad = malloc(sizeof(int) * ARRAY_SIZE);

    for (i=0; i<ARRAY_SIZE; i++) {
        aa[i] = rand() % 100;
    }

    memcpy(ab, aa, sizeof(int) * ARRAY_SIZE);
    memcpy(ac, aa, sizeof(int) * ARRAY_SIZE);
    memcpy(ad, aa, sizeof(int) * ARRAY_SIZE);

    /*cnt = 0;
    gettimeofday(&t1, 0);
    bubble(aa, ARRAY_SIZE);
    gettimeofday(&t2, 0);
    printf("Bubble took %ld swaps (%ldms.)\n", cnt, get_elapsed_ms(t1, t2));

    cnt = 0;
    gettimeofday(&t1, 0);
    flat(ab, ARRAY_SIZE);
    gettimeofday(&t2, 0);
    printf("Flat took %ld swaps (%ldms.)\n", cnt, get_elapsed_ms(t1, t2));*/

    cnt = 0;
    gettimeofday(&t1, 0);
    quicksort(ac, 0, ARRAY_SIZE-1);
    gettimeofday(&t2, 0);
    printf("QuickSort took %ld swaps (%ldms.)\n", cnt, get_elapsed_ms(t1, t2));

    cnt = 0;
    gettimeofday(&t1, 0);
    quicksort2(ac, 0, ARRAY_SIZE-1);
    gettimeofday(&t2, 0);
    printf("QuickSort2 took %ld swaps (%ldms.)\n", cnt, get_elapsed_ms(t1, t2));

    free(aa);
    free(ab);
    free(ac);
    free(ad);

    return 0;
}

void init(int *arr, int size) {
    int i;
    cnt = 0;

    for (i=0; i<size; i++) {
        arr[i] = rand() % 100;
    }
}

long get_elapsed_ms(struct timeval t1, struct timeval t2) {
    return ((t2.tv_sec - t1.tv_sec) * 1000 + (t2.tv_usec - t1.tv_usec) / 1000);
}

void swap(int *a, int *b) {
    int tmp;

    cnt++;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

void quicksort(int *arr, int left, int right) {
    int i = left, j = right;
    int pivot = arr[(left + right) / 2];

    /* partition */
    while (i <= j) {
        while (arr[i] < pivot)
            i++;
        while (arr[j] > pivot)
            j--;
        if (i <= j) {
            swap(&arr[i], &arr[j]);
            i++;
            j--;
        }
    }

    /* recursion */
    if (left < j)
        quicksort(arr, left, j);
    if (i < right)
        quicksort(arr, i, right);
}

void quicksort2(int *arr, int l, int u) {
    int i, m, rnd;
    if (l >= u)
        return;

    rnd = rand() % (u - l) + l;
    swap(&arr[l], &arr[rnd]);

    m = l;

    for (i = l+1; i <= u; i++)
        if (arr[i] < arr[l])
            swap(&arr[++m], &arr[i]);

    swap(&arr[l], &arr[m]);
    quicksort2(arr, l, m-1);
    quicksort2(arr, m+1, u);
}


void bubble(int *arr, int size) {
    int i, s = -1;

    while (s != 0) {
        s = 0;

        for (i=0; i<size-1; i++) {
            if (arr[i] > arr[i+1]) {
                swap(&arr[i], &arr[i+1]);
                s++;
            }
        }
    }
}

void flat(int *arr, int size) {
    int i, j;

    for (i=0; i<size-1; i++) {
        for (j=i+1; j<size; j++) {
            if (arr[i] > arr[j]) {
                swap(&arr[i], &arr[j]);
            }
        }
    }
}
