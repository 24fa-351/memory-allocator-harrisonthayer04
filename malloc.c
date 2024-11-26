#include "malloc.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct memoryBlock {
    void* ptrToData;
    size_t blockSize;
    bool blockIsFree;
} memoryBlock;

#define MAX 17592186044416

static memoryBlock blocks[MAX];
static int numberOfBlocks = 0;

// Merges two adjacent blocks if they are both free, ensuring that the memory in
// our array doesn't get fragmented.
void mergeBlocks() {
    for (int i = 0; i < numberOfBlocks - 1;) {
        if (blocks[i].blockIsFree) {
            void* currentEnd = (char*)blocks[i].ptrToData + blocks[i].blockSize;
            if (blocks[i + 1].blockIsFree &&
                (char*)blocks[i + 1].ptrToData == currentEnd) {
                blocks[i].blockSize += blocks[i + 1].blockSize;
                for (int j = i + 1; j < numberOfBlocks - 1; j++) {
                    blocks[j] = blocks[j + 1];
                }
                numberOfBlocks--;
            } else {
                i++;
            }
        } else {
            i++;
        }
    }
}

void* xmalloc(size_t blockSize) {
    mergeBlocks();

    // First, Loop over our blocks, if we find a block that is a. Free and b.
    // the exact size we are looking for, we will return it.
    for (int i = 0; i < numberOfBlocks; i++) {
        if (blocks[i].blockIsFree && blocks[i].blockSize == blockSize) {
            blocks[i].blockIsFree = false;
            return blocks[i].ptrToData;
        }
    }

    // If there are no exact matches, we will look for a larger block, and split
    // it into two. One block of the desired size, and another block of the
    // remaining size.
    for (int i = 0; i < numberOfBlocks; i++) {
        if (blocks[i].blockIsFree && blocks[i].blockSize > blockSize) {
            void* newPtrToData = blocks[i].ptrToData;
            size_t remainingBlockSize = blocks[i].blockSize - blockSize;

            blocks[i].blockSize = blockSize;
            blocks[i].blockIsFree = false;

            if (numberOfBlocks >= MAX) {
                fprintf(stderr,
                        "Error: Maximum number of memory blocks reached\n");
                return NULL;
            }

            blocks[numberOfBlocks].ptrToData = (char*)newPtrToData + blockSize;
            blocks[numberOfBlocks].blockSize = remainingBlockSize;
            blocks[numberOfBlocks].blockIsFree = true;
            numberOfBlocks++;

            return newPtrToData;
        }
    }

    // If we were unable to find a suitable block, we will ask the operating
    // system for more memory so we can create a new block. We will then add
    // this block to our existing list of blocks and return the pointer to the
    // data.

    void* newPtrToData = sbrk(blockSize);
    blocks[numberOfBlocks].ptrToData = newPtrToData;
    blocks[numberOfBlocks].blockSize = blockSize;
    blocks[numberOfBlocks].blockIsFree = false;
    numberOfBlocks++;
    return newPtrToData;
}

void xfree(void* ptrToData) {
    // Loop over all the blocks, if the desired pointer is found, and in use,
    // free it and return.
    for (int i = 0; i < numberOfBlocks; i++) {
        if (blocks[i].ptrToData == ptrToData && !blocks[i].blockIsFree) {
            blocks[i].blockIsFree = true;
            break;
        }
    }
    mergeBlocks();
    return;
}