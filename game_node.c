#include "game_node.h"

#include "frame.h"
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
    new_node->search_penalty = 0;

    // Mark all the children as NULL and the preferred children
    // as 8 (out of bounds).
    size_t cidx;
    for (cidx = 0; cidx < 7; cidx++) {
	new_node->children[cidx] = NULL;
    }
    new_node->preferred_move_child = 8;
    new_node->preferred_search_child = 8;

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

char expand_node(struct GameNode* node) {
    // Create and add the children node for the current node.
    // Then propagate the new payoff information all the way
    // back up the tree.
    int cidx;

    // Use this variable to note whether or not there
    // are any children (to determine if the frame is full).
    char has_children;
    has_children = 0;

    // already_won denotes whether someone has already won in
    // the current frame position. It is our return value.
    char already_won;
    already_won = 0;

    struct GameNode* child_node;
    struct FramePosition* child_position;

    // If someone has already won (not just if the baord is
    // full) we don't need to go through this rigamarole.
    // Just return, and note in the return value that the
    // game is over.
    // We don't need to do any updating because the fact that
    // this node is won was already handled when it was
    // first created.
    if (check_winner(node->position) != 0) {
	already_won = 1;
    } else {
	// Add each child.
	// Valgrind helped me find a memory leak in the first
	// implementation of this where I created each child node
	// before checking if that column was full. Then, if the
	// column was full, I orphaned the child.
	for (cidx = 0; cidx < 7; cidx++) {
	    // Only add this child if the top cell is 0.
	    if (get_at_col_row(node->position, cidx, 5) == 0) {
		has_children = 1;
		
		// Create child position.
		child_position = deep_copy_frame(node->position);
		
		// Make the move. This should be valid because the top
		// cell is empty!
		move_in_col(child_position, cidx);
		
		// Create the child node to hold the child position, and
		// add it to the current node's array of children.
		child_node = new_game_node(node, child_position);
		node->children[cidx] = child_node;
	    }
	}
	
	if (has_children == 0) {
	    // The board is full. We should not update for this
	    // node. Instead, give it a penalty and update its
	    // parent.
	    node->search_penalty = WINNING_SCORE / 2;
	    update_expected_payoff(node->parent);
	} else {
	    // Update the expected payoff up the tree, recursively.
	    update_expected_payoff(node);
	}
    }

    return already_won;
}

void update_expected_payoff(struct GameNode* node) {
    // Choose your new expected payoff to be max or min of
    // your children's. Then make your parent update also.

    // We assume that the current position has children
    // (because it is not tied or won).
    
    // optimal_payoff should start out as un-optimal as possible
    // (i.e., assuming that we lose immediately). Because we
    // assume that we have children, this won't end up being
    // the final result.
    int best_search_sum;
    best_search_sum = 0;
    if (get_to_move(node->position) == 1) {
	// The worst possible scenario would be -WINNING_SCORE.
	node->expected_payoff = -WINNING_SCORE;
	best_search_sum = -2*WINNING_SCORE;
    } else {
	node->expected_payoff = WINNING_SCORE;
	best_search_sum = 2*WINNING_SCORE;
    }

    int child_payoff;

    int child_idx;
    for (child_idx = 0; child_idx < 7; child_idx++) {
	if (node->children[child_idx] != NULL) {
	    child_payoff = node->children[child_idx]->expected_payoff;

	    // First update our expected_payoff based on the
	    // children's.
	    // Check if child's payoff is better than we've seen so far.
	    if (((get_to_move(node->position) == 1) &&
		 (child_payoff >= node->expected_payoff)) ||
		((get_to_move(node->position) == 2) &&
		 (child_payoff <= node->expected_payoff))) {
		// This should be the new optimal_payoff, and this child
		// is the new most promising.
		node->expected_payoff = child_payoff;
		node->preferred_move_child = child_idx;
	    }

	    // Next, update our search_payoff and penalty based
	    // on the children's.
	    if (((get_to_move(node->position) == 1) &&
		 (node->children[child_idx]->search_payoff -
		  node->children[child_idx]->search_penalty >=
		  best_search_sum)) || 
		((get_to_move(node->position) == 2) &&
		 (node->children[child_idx]->search_payoff +
		  node->children[child_idx]->search_penalty <=
		  best_search_sum))) {
		    // This is the new most promising child
		    // node for searching.
		    node->preferred_search_child = child_idx;
		    node->search_payoff =
			node->children[child_idx]->search_payoff;
		    node->search_penalty =
			node->children[child_idx]->search_penalty;
		}
	}
    }
    
    if (node->parent != NULL) {
	update_expected_payoff(node->parent);
    }
}

struct GameNode* find_preferred_search_leaf(struct GameNode* root) {
    // The only time we use this is to pick which node we want to
    // expand next. Thus, this uses the search payoffs, not the
    // actual expected payoffs. To determine the best move you
    // simply use the preferred_move_child field of the game_node.
    struct GameNode* current_node;
    current_node = root;

    while (1) {
	if (current_node->preferred_move_child == 8) {
	    // This node has not yet been expanded, so it's a leaf node.
	    break;
	} else {
	    current_node = current_node->children[current_node->
						  preferred_move_child];
	}
    }
    
    return current_node;
}

int get_best_move(struct GameNode* node) {
    return node->preferred_move_child;
}
