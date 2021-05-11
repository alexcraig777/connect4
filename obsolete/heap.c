#include "heap.h"

#include <stdlib.h>


struct Heap* new_heap(size_t space) {
    struct Heap* rtn;
    struct HeapNode** root;

    rtn = malloc(sizeof(struct Heap));

    root = malloc(sizeof(struct HeapNode*) * space);

    rtn->space = space;
    rtn->num_items = 0;
    rtn->root = root;

    return rtn;
}

void heap_insert(struct Heap* heap, struct HeapNode* new_node) {
    // Insert this node into the heap maintaining max-heap property.
    // If necessary, allocate more space for the heap's array.

    // First, check if we need to allocate more space.
    if (heap->num_items == heap->space) {
	// Allocate a new array.
	struct HeapNode** new_root;
	new_root = malloc(sizeof(struct HeapNode*) * 2 * heap->space);

	// Copy the heap array over.
    size_t i;
	for (i = 0; i < heap->num_items; i++) {
	    new_root[i] = heap->root[i];
	}

	// Free the old heap array.
	free(heap->root);

	// Attach the new array to the heap.
	heap->root = new_root;
    }

    // Now insert the new item at the end and up_heapify.
    heap->root[heap->num_items] = new_node;
    heap->num_items++;

    up_heapify(heap, (heap->num_items) - 1);
}

struct HeapNode* heap_pop(struct Heap* heap) {
    // Remove and return the max item (root) from the heap.
    // Do this by replacing the root with the last element
    // and then down_heapifying.
    struct HeapNode* rtn;

    if (heap->num_items == 0) {
	rtn = NULL;
    } else {
	rtn = heap->root[0];

	// Move last element up to the front.
	heap->root[0] = heap->root[(heap->num_items) - 1];
	
	// Note that we now have 1 fewer elements.
	heap->num_items--;
    
	down_heapify(heap, 0);
    }
    
    return rtn;
}
    

void up_heapify(struct Heap* heap, int idx) {
    // Starting at indx idx into the heap, fix the max-heap
    // property going up.
    int parent_idx = (idx - 1)/2;
    
    struct HeapNode *current, *parent;

    // If idx = 0, we're automatically done.
    if (idx != 0) {
	current = heap->root[idx];
	parent = heap->root[parent_idx];

	if (parent->key < current->key) {
	    // Swap parent and child and continue up.
	    heap->root[idx] = parent;
	    heap->root[parent_idx] = current;
	    up_heapify(heap, parent_idx);
	}
    }
}

void down_heapify(struct Heap* heap, int idx) {
    // Starting at index idx into the heap, make sure that the
    // heap satisifies the max-heap property for this subtree.

    size_t left_idx, right_idx;
    struct HeapNode *parent, *left, *right;
    parent = heap->root[idx];

    left_idx = 2*idx + 1;
    right_idx = 2*idx + 2;

    // If the right idx is within bounds, check if we need to
    // swap with either.
    if (right_idx < heap->num_items) {
	left = heap->root[left_idx];
	right = heap->root[right_idx];
	if (parent->key < left->key) {
	    // Left child beats parent.
	    if (left->key < right->key) {
		// Right child beats left child.
		// We need to swap right child up and parent down.
		heap->root[idx] = right;
		heap->root[right_idx] = parent;
		down_heapify(heap, right_idx);
	    } else {
		// Swap left child and parent.
		heap->root[idx] = left;
		heap->root[left_idx] = parent;
		down_heapify(heap, left_idx);
	    }
	} else if (parent->key < right->key) {
	    // Need to swap right child and parent.
	    heap->root[idx] = right;
	    heap->root[right_idx] = parent;
	    down_heapify(heap, right_idx);
	}
    } else if (left_idx < heap->num_items) {
	// The left child is the very last item in the heap.
	left = heap->root[left_idx];
	if (parent->key < left->key) {
	    // Swap left child with parent. No need to down_heapify
	    // again.
	    heap->root[idx] = left;
	    heap->root[left_idx] = parent;
	}
    }
}
