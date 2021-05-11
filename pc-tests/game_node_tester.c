#include "../frame.h"
#include "../game_node.h"

#include <stdio.h>

void display_frame(struct FramePosition* position) {
    int cidx, ridx;
    char* display_symbols[3] = {".", "x", "o"};

    printf("\n");
    for (ridx = 5; ridx >= 0; ridx--) {
	for (cidx = 0; cidx < 7; cidx++) {
	    printf("%s ", display_symbols[(int) position->cells[cidx][ridx]]);
	}
	printf("\n");
    }
}

void display_node(struct GameNode* node) {
    printf("\nNode\n");

    display_frame(node->position);

    printf("Expected payoff:\t%d\n", node->expected_payoff);
}

int main() {
    struct FramePosition* frame = new_frame();

    struct GameNode* root;
    struct GameNode* leaf;

    int user_input_col;
    
    int i, j;
    for (i = 0; i < 100; i++) {
	root = new_game_node(NULL, frame);

	for (j = 0; j < 100; j++) {
	    //printf("Root:");
	    //display_node(root);
	    
	    leaf = find_most_promising_leaf_descendant(root);
	    
	    //printf("Leaf:");
	    //display_node(leaf);
	    
	    expand_node(leaf);
	
	    //printf("Root again:");
	    //display_node(root);
	}

	move_in_col(root->position, root->most_promising_child);

	frame = deep_copy_frame(root->position);

	free_game_node(root);

	printf("Computer made a move. Now the position is\n");

	display_frame(frame);

	printf("\nYour move:\t");
	scanf("%d", &user_input_col);

	printf("You made the move %d.\n", user_input_col);

	move_in_col(frame, user_input_col);

	display_frame(frame);
    }

    /* move_in_col(frame, 3);
    move_in_col(frame, 3);
    move_in_col(frame, 2);
    move_in_col(frame, 3);
    move_in_col(frame, 3);
    display_frame(frame); */

    return 0;
}
