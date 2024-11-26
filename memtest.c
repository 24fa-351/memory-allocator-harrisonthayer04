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

char letters[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                    'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                    's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

char* combineCharsIntoStrings(int stringSize, int letterIndicies[]) {
    char* string = xmalloc(stringSize + 1);
    for (int i = 0; i < stringSize; i++) {
        string[i] = letters[letterIndicies[i]];
    }
    string[stringSize] = '\0';
    // fprintf(stderr, "String generated: %s\n", string); // (used for
    // debugging)
    return string;
}

int main() {
    srand(time(NULL) ^ getpid());
    fprintf(stderr, "Creating array of size %i \n", TEST_SIZE);
    char** array = xmalloc(TEST_SIZE * sizeof(char*));
    fprintf(stderr, "Array has been created\n");
    fprintf(stderr, "Filling array with %i strings\n", TEST_SIZE);

    for (int i = 0; i < TEST_SIZE; i++) {
        int stringSize = rand() % 10 + 1;
        int letterIndicies[stringSize];
        for (int j = 0; j < stringSize; j++) {
            letterIndicies[j] = rand() % 26;
        }
        char* string = combineCharsIntoStrings(stringSize, letterIndicies);
        array[i] = string;
    }
    fprintf(stderr, "Array has been filled, printing the array.\n");
    for (int i = 0; i < TEST_SIZE; i++) {
        fprintf(stderr, "[%i]: %s\n", (i + 1), array[i]);
    }
    fprintf(stderr, "Freeing the array\n");
    for (int i = 0; i < TEST_SIZE; i++) {
        xfree(array[i]);
    }
    xfree(array);

    fprintf(stderr, "Current File: %s, Current Line: %d \n", __FILE__,
            __LINE__);

    return 0;
}