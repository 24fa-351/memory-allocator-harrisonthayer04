#include "malloc.h"
#include "heap.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct memoryBlock {
    void* ptrToData;
    size_t blockSize;
    bool blockIsFree;
} memoryBlock;

#define MAX 1000000

static memoryBlock blocks[MAX];
static int numberOfBlocks = 0;

void* getLargerBlock(size_t blockSize) {
    if (numberOfBlocks >= MAX) {
        fprintf(stderr, "Error: Maximum number of memory blocks reached\n");
        return NULL;
    }
    void* newPtrToData = sbrk(blockSize);
    blocks[numberOfBlocks].ptrToData = newPtrToData;
    blocks[numberOfBlocks].blockSize = blockSize;
    blocks[numberOfBlocks].blockIsFree = false;
    numberOfBlocks++;
    return newPtrToData;
}

// Merges two adjacent blocks if they are both free, ensuring that the memory in
// our array doesn't get fragmented.
void mergeBlocks() {
    for (int ia = 0; ia < numberOfBlocks - 1;) {
        if (blocks[ia].blockIsFree) {
            void* currentEnd = (char*)blocks[ia].ptrToData + blocks[ia].blockSize;
            if (blocks[ia + 1].blockIsFree &&
                (char*)blocks[ia + 1].ptrToData == currentEnd) {
                blocks[ia].blockSize += blocks[ia + 1].blockSize;
                for (int ja = ia + 1; ja < numberOfBlocks - 1; ja++) {
                    blocks[ja] = blocks[ja + 1];
                }
                numberOfBlocks--;
            } else {
                ia++;
            }
        } else {
            ia++;
        }
    }
}

void* xmalloc(size_t blockSize) {
    mergeBlocks();

    // First, Loop over our blocks, if we find a block that is a. Free and b.
    // the exact size we are looking for, we will return it.
    for (int ib = 0; ib < numberOfBlocks; ib++) {
        if (blocks[ib].blockIsFree && blocks[ib].blockSize == blockSize) {
            blocks[ib].blockIsFree = false;
            return blocks[ib].ptrToData;
        }
    }

    // If there are no exact matches, we will look for a larger block, and split
    // it into two. One block of the desired size, and another block of the
    // remaining size.
    for (int ic = 0; ic < numberOfBlocks; ic++) {
        if (blocks[ic].blockIsFree && blocks[ic].blockSize > blockSize) {
            void* newPtrToData = blocks[ic].ptrToData;
            size_t remainingBlockSize = blocks[ic].blockSize - blockSize;

            blocks[ic].blockSize = blockSize;
            blocks[ic].blockIsFree = false;

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

    void* newPtrToData = getLargerBlock(blockSize);
    return newPtrToData;
}

void xfree(void* ptrToData) {
    // Loop over all the blocks, if the desired pointer is found, and in use,
    // free it and return.
    for (int id = 0; id < numberOfBlocks; id++) {
        if (blocks[id].ptrToData == ptrToData && !blocks[id].blockIsFree) {
            blocks[id].blockIsFree = true;
            break;
        }
    }
    mergeBlocks();
    return;
}