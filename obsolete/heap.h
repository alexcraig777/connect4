#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>

struct HeapNode {
    void* object;
    int key;
};

struct Heap {
    size_t space;
    size_t num_items;
    struct HeapNode** root;
};


struct Heap* new_heap(size_t space);

void heap_insert(struct Heap* heap, struct HeapNode* new_node);

struct HeapNode* heap_pop(struct Heap* heap);

void up_heapify(struct Heap* heap, int idx);

void down_heapify(struct Heap* heap, int idx);

#endif
