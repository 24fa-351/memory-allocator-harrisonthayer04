
#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

#define KEY_NOT_PRESENT -1
#define IS_EVEN(x) (((x)&1) == 0)

heap_t *heap_create(int capacity) {
  heap_t *temp = malloc(sizeof(heap_t));
  temp->data = malloc(sizeof(heap_node_t) * capacity);
  temp->size = 0;
  temp->capacity = capacity;
  return temp;
}

void heap_free(heap_t *heap) {
  free(heap->data);
  free(heap);
}

unsigned int heap_size(heap_t *heap) { return heap->size; }

unsigned int heap_parent(unsigned int index) {
  if (IS_EVEN(index)) {
    return ((index / 2) - 1);
  } else {
    return (index / 2);
  }
}

unsigned int heap_left_child(unsigned int index) { return ((2 * index) + 2); }

unsigned int heap_right_child(unsigned int index) { return ((2 * index) + 1); }

unsigned int heap_level(unsigned int index) {
  if (index == 0) {
    return 0;
  }
  if (index == 1 || index == 2) {
    return 1;
  }

  unsigned int steps = 1;
  while (index > 2) {
    steps++;
    index = heap_parent(index);
  }
  return steps;
}

void heap_print(heap_t *heap) {
  for (int ix = 0; ix < heap_size(heap); ix++) {
    printf("%3d - %3d : " HEAP_KEY_FORMAT "\n", heap_level(ix), ix,
           heap->data[ix].key);
  }
  printf("\n");
}

void heap_swap(heap_t *heap, int index1, int index2) {
  heap_node_t temp = heap->data[index1];
  heap->data[index1] = heap->data[index2];
  heap->data[index2] = temp;
}

void heap_bubble_up(heap_t *heap, int index) {
  while (index > 0) {
    if ((heap->data[index].key) < (heap->data[heap_parent(index)].key)) {
      heap_swap(heap, index, heap_parent(index));
      index = heap_parent(index);
    } else {
      break;
    }
  }
}

void heap_bubble_down(heap_t *heap, int index) {
  while (1) {
    unsigned int rightChild = heap_right_child(index);
    unsigned int leftChild = heap_left_child(index);
    unsigned int current = index;

    if (leftChild < heap_size(heap) &&
        heap->data[leftChild].key < heap->data[current].key) {
      current = leftChild;
    }
    if (rightChild < heap_size(heap) &&
        heap->data[rightChild].key < heap->data[current].key) {
      current = rightChild;
    }
    if (current != index) {
      heap_swap(heap, index, current);
      index = current;
    } else {
      break;
    }
  }
}

void heap_insert(heap_t *heap, heap_key_t key, heap_value_t data) {
  if (heap_size(heap) == heap->capacity) {
    return;
  }

  heap->data[heap_size(heap)].key = key;
  heap->data[heap_size(heap)].value = data;
  heap->size++;

  heap_bubble_up(heap, heap_size(heap) - 1);
}

heap_value_t heap_remove_min(heap_t *heap) {
  if (heap_size(heap) == 0) {
    return NULL;
  }

  heap_value_t min = heap->data[0].value;

  heap->size--;

  // move last element to the root
  heap->data[0] = heap->data[heap_size(heap)];

  // then bubble it down to its correct position
  heap_bubble_down(heap, 0);

  return min;
}
