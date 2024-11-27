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

    fprintf(stderr, "Allocating large blocks\n");
    int* largeBlock1 =
        xmalloc(1000000 * sizeof(int));  // 4 bytes * 1,000,000 = 4MB
    fprintf(stderr, "Large block 1 (4MB) has been allocated\n");
    xfree(largeBlock1);
    fprintf(stderr, "Large block 1 (4MB) has been freed\n");

    int* largeBlock2 =
        xmalloc(30000000 * sizeof(int));  // 4 bytes * 30,000,000 = 120MB
    fprintf(stderr, "Large block 2 (120MB) has been allocated\n");
    xfree(largeBlock2);
    fprintf(stderr, "Large block 2 (120MB) has been freed\n");

    int* largeBlock3 =
        xmalloc(250000000 * sizeof(int));  // 4 bytes * 250,000,000 = 1GB
    fprintf(stderr, "Large block 3 (1GB) has been allocated\n");
    xfree(largeBlock3);
    fprintf(stderr, "Large block 3 (1GB) has been freed\n");

    return 0;
}