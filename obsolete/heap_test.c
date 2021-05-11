#include "heap.h"

#include <stdio.h>
#include <stdlib.h>

void print_heap(struct Heap* heap) {
    printf("Heap:\t");
    printf("space: %d\t", heap->space);
    printf("items: %d\t\n", heap->num_items);

    for (int i = 0; i < heap->num_items; i++) {
	printf("%d\t", heap->root[i]->key);
    }
    printf("\n");
}

int main() {
    // Create a heap and fill it with some heap nodes!
    struct Heap* heap;
    heap = new_heap(4);

    print_heap(heap);

    struct HeapNode* heap_node;
    for (int i = 0; i < 10; i++) {
	heap_node = malloc(sizeof(struct HeapNode));
	heap_node->object = NULL;
	heap_node->key = i*i % 9;

	heap_insert(heap, heap_node);
    }

    print_heap(heap);
    printf("\n");
    
    for (int i = 0; i < 10; i++) {
	heap_node = heap_pop(heap);

	printf("%d\n", heap_node->key);
    }

    printf("\n");
    print_heap(heap);

    return 0;
}
