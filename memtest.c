#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#ifdef SYSTEM_MALLOC
#define xfree free
#define xmalloc malloc
#else
#include "malloc.h"
#endif

#define TEST_SIZE 500

int main() {
    srand(time(NULL) ^ getpid());
    fprintf(stderr, "Creating array of size %i \n", TEST_SIZE);
    int* array = xmalloc(TEST_SIZE * sizeof(int));
    fprintf(stderr, "Array has been created\n");
    fprintf(stderr, "Filling array with %i integers\n", TEST_SIZE);
    for (int ia = 0; ia < TEST_SIZE; ia++) {
        int temp = rand() % 10000;
        array[ia] = temp;
    }
    fprintf(stderr, "Array has been filled, printing the array.\n");
    for (int ib = 0; ib < TEST_SIZE; ib++) {
        fprintf(stderr, "[%i]: %d\n", (ib + 1), array[ib]);
    }
    fprintf(stderr, "Freeing the array\n");
    xfree(array);
    return 0;
}