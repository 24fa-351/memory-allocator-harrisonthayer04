
#ifndef HEAP_H
#define HEAP_H

// a min heap

typedef unsigned long long heap_key_t;
typedef void* heap_value_t;
#define HEAP_KEY_FORMAT "%llu"

typedef struct {
    heap_key_t key;
    heap_value_t value;
} heap_node_t;

typedef struct {
    heap_node_t *data;
    int size;
    int capacity;
} heap_t;

heap_t *heap_create(int capacity);

void heap_free(heap_t *heap);

unsigned int heap_size(heap_t *heap);

unsigned int heap_parent(unsigned int index);

unsigned int heap_left_child(unsigned int index);

unsigned int heap_right_child(unsigned int index);

unsigned int heap_level(unsigned int index);

void heap_swap(heap_t *heap, int index1, int index2);

void heap_bubble_up(heap_t *heap, int index);

void heap_bubble_down(heap_t *heap, int index);

void heap_insert(heap_t *heap, heap_key_t key, heap_value_t data);

heap_value_t heap_remove_min(heap_t *heap);

void heap_print(heap_t *heap);

#endif
