#include "game_node.h"

#include "small_frame.h"
#include "frame_ops.h"

#include <stdlib.h>
#include <stddef.h>

#define WINNING_SCORE 10000

struct GameNode* new_game_node(struct GameNode* parent,
			       struct FramePosition* position) {
    struct GameNode* new_node;
    new_node = malloc(sizeof(struct GameNode));

    // Fill in the required standard fields.
    new_node->position = position;
    new_node->parent = parent;
    new_node->expected_payoff = score_frame(position, 1);

    // Mark all the children as NULL and the most_promising_child
    // as 8 (out of bounds).
    size_t cidx;
    for (cidx = 0; cidx < 7; cidx++) {
	new_node->children[cidx] = NULL;
    }
    new_node->most_promising_child = 8;

    return new_node;
}

void free_game_node(struct GameNode* node) {
    // Free all memory associated with this game node and all of
    // its children!

    // First, free all children (using this same function recursively),
    // which will recursively get the whole subtree.
    int child_idx;
    for (child_idx = 0; child_idx < 7; child_idx++) {
	if (node->children[child_idx] != NULL) {
	    free_game_node(node->children[child_idx]);
	}
    }

    // Now free the FramePosition using the special FramePosition
    // free routine.
    free_frame(node->position);

    // Finally, free the actual GameNode structure itself.
    free(node);
}

void expand_node(struct GameNode* node) {
    // Create and add the children node for the current node.
    // Then propagate the new payoff information all the way
    // back up the tree.
    int cidx;

    struct GameNode* child_node;
    struct FramePosition* child_position;
    
    // Add each child.
    for (cidx = 0; cidx < 7; cidx++) {
	// Create child position.
	child_position = deep_copy_frame(node->position);
	
	if (move_in_col(child_position, cidx) != -1) {
	    // Create a new child node.
	    child_node = new_game_node(node, child_position);

	    // Add the child node to the current node's children array.
	    node->children[cidx] = child_node;
	} else {
	    // We need to free the child_position frame that we
	    // just created!
	    // This was causing some memory leaks!
	    // Thank you Valgrind for helping me find it!
	    free_frame(child_position);
	}
    }

    // Update the expected payoff up the tree, recursively.
    update_expected_payoff(node);
}

void update_expected_payoff(struct GameNode* node) {
    // Choose your new expected payoff to be max or min of
    // your children's. Then make your parent update also.
    
    int child_payoff;
    // optimal_payoff should start out as un-optimal as possible
    // (i.e., assuming that we lose immediately).
    if (get_to_move(node->position) == 1) {
	// The worst possible scenario would be -WINNING_SCORE.
	node->expected_payoff = -WINNING_SCORE;
    } else {
	node->expected_payoff = WINNING_SCORE;
    }

    int child_idx;
    for (child_idx = 0; child_idx < 7; child_idx++) {
	if (node->children[child_idx] != NULL) {
	    child_payoff = node->children[child_idx]->expected_payoff;
	    // Check if child's payoff is better than we've seen so far.
	    if (((get_to_move(node->position) == 1) &&
		 (child_payoff >= node->expected_payoff)) ||
		((get_to_move(node->position) == 2) &&
		 (child_payoff <= node->expected_payoff))) {
		// This should be the new optimal_payoff, and this child
		// is the new most promising.
		node->expected_payoff = child_payoff;
		node->most_promising_child = child_idx;
	    }
	    // Otherwise this child doesn't add anything new.
	}
    }
    
    if (node->parent != NULL) {
	update_expected_payoff(node->parent);
    }
}

struct GameNode* find_most_promising_leaf_descendant(struct GameNode* root) {
    struct GameNode* current_node;
    current_node = root;

    while (1) {
	if (current_node->most_promising_child == 8) {
	    // This node has not yet been expanded, so it's a leaf node.
	    break;
	} else {
	    current_node = current_node->children[current_node->
						  most_promising_child];
	}
    }
    
    return current_node;
}
