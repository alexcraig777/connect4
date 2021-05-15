#include "../frame.h"
#include "../game_node.h"

#include <stdio.h>

#define MAX_EXPANSIONS 1000

void display_frame(struct FramePosition* position) {
    int cidx, ridx;
    char* display_symbols[3] = {".", "x", "o"};

    printf("\n");
    for (ridx = 5; ridx >= 0; ridx--) {
	for (cidx = 0; cidx < 7; cidx++) {
	    printf("%s ", display_symbols[get_at_col_row(position, cidx, ridx)]);
	}
	printf("\n");
    }
}

void display_node(struct GameNode* node) {
    printf("\nNode\n");

    display_frame(node->position);

    printf("Expected payoff:\t%d\n", node->expected_payoff);
}

void display_winner(int winner) {
    if (winner == 1) {
	printf("Computer won!\nBetter luck next time!\n");
    } else  if (winner == 2) {
	printf("Congratulations!\nYou beat the computer!\n");
    }	
}

int main() {
    struct FramePosition* frame = new_frame();

    struct GameNode* root;
    struct GameNode* leaf;

    int winner;
    
    int user_input_col;
    
    int i, j;
    for (i = 0; i < 1000; i++) {
	root = new_game_node(NULL, frame);

	for (j = 0; j < MAX_EXPANSIONS; j++) {
	    //printf("Root:");
	    //display_node(root);
	    
	    leaf = find_most_promising_leaf_descendant(root);

	    // Check if the leaf is the end of the game.
	    winner = check_winner(leaf->position);
	    if (winner != 0) {
		printf("Looks like somebody's got a forced win!\n");
		break;
	    }
	    
	    expand_node(leaf);
	}

	// Create a copy of the current frame.
	frame = deep_copy_frame(root->position);

	// Make the computed best move.
	move_in_col(frame, root->most_promising_child);

	// Free the previous root game node (and with it the entire tree).
	free_game_node(root);

	// Display the new position.
	printf("Computer made a move. Now the position is\n");
	display_frame(frame);

	// Now check to see if the computer just won or filled up the board.
	// In either case we need gracefully to stop and break out of
	// the loop.
	winner = check_winner(frame);
	if (winner != 0) {
	    display_winner(winner);
	    break;
	} else if (check_full(frame)) {
	    printf("Game over. The board is full.\n");
	    break;
	}
	
	// Get the user input and make their move.
	printf("\nYour move:\t");
	scanf("%d", &user_input_col);

	printf("You made the move %d.\n", user_input_col);

	move_in_col(frame, user_input_col);

	display_frame(frame);

	// Check if there's a winner or the frame is full. In either case
	// we need to end gracefully and then break out of the loop.
	winner = check_winner(frame);
	if (winner != 0) {
	    display_winner(winner);
	    break;
	} else if (check_full(frame)) {
	    printf("Game over. The board is full.\n");
	    break;
	}
	
    } // End main game loop.
    
    // Free the current frame object.
    free_frame(frame);

    return 0;
}
