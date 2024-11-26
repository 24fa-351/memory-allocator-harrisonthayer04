#include <stdlib.h>

#ifndef MALLOC_H
#define MALLOC_H

void mergeBlocks();
void* xmalloc(size_t size);
void xfree(void* ptr);

#endif