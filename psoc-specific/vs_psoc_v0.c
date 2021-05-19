#include <project.h>
#include "tft.h"
#include "graphics.h"
#include "led_control.h"

#include "../frame.h"
#include "../frame_ops.h"
#include "../game_node.h"

#include <stdlib.h>

#define MAX_EXPANSIONS 70

struct FramePosition* frame;
char overall_winner;
int score;

int execute_user_move() {
    // Wait for the user's move (i.e., for user to press
    // one of the 7 column buttons), and then update the
    // frame accordingly.
    // Return the column in which the player moved.
    int move_col;
    int move_rtn_val;
    
    while (1) {
        move_col = 8;
        if (Col_0_Read()) {
            move_col = 0;
        } else if (Col_1_Read()) {
            move_col = 1;
        } else if (Col_2_Read()) {
            move_col = 2;
        } else if (Col_3_Read()) {
            move_col = 3;
        } else if (Col_4_Read()) {
            move_col = 4;
        } else if (Col_5_Read()) {
            move_col = 5;
        } else if (Col_6_Read()) {
            move_col = 6;
        }
        
        if (move_col != 8) {
            move_rtn_val = move_in_col(frame, move_col);
            if (move_rtn_val == 0) {
                // We just execited a valid move.
                break;
            } else {
                // The user input an invalid move.
                signal_illegal_move();
            }
        }
    }
    return move_col;
}

int main() {
    int mov_col, mov_row;
    mov_row = 0;

    // Start the clock/PWM hardware.
    Clock_1_Enable();
    Clock_2_Enable();
	PWM_1_Start();
	PWM_2_Start();
    
    // Set the LED to breathing.
    led_breathe();
    
    struct GameNode* root;
    struct GameNode* leaf;

    char winner;
    
    int search_counter;
    
    // Enable global interrupts.
    CyGlobalIntEnable;
    
    // Initialize SPIM component and the TFT display.
    SPIM_1_Start();
    tftStart();
    
    frame = new_frame();
    
    display_frame(frame);
    
    display_winner(0);
    
    // Initialize the interrupt from the user buttons.
    //USER_INPUT_StartEx(input_handler);
    
    while (1) {
    	// Initialize a game node for the current position.
    	root = new_game_node(NULL, frame);

    	// Repeatedly search the tree descending from root.
    	for (search_counter = 0; search_counter < MAX_EXPANSIONS;
	     search_counter++) {
    	    leaf = find_preferred_search_leaf(root);

    	    winner = check_winner(leaf->position);
    	    if (winner != 0) {
        		// Somebody has a forced win from this position!
        		break;
    	    }

    	    expand_node(leaf);
    	}

    	// Create a copy of the current frame.
    	frame = deep_copy_frame(root->position);

    	// Make the computed best move.
        mov_col = get_best_move(root);
    	move_in_col(frame, mov_col);

    	// Free the previous root game node (and with it the entire tree).
    	free_game_node(root);

        // Figure out which row the piece was placed in,
        // so that we can animate the piece falling in.
        int ridx;
        for (ridx = 5; ridx >= 0; ridx--) {
            if (get_at_col_row(frame, mov_col, ridx) != 0) {
                mov_row = ridx;
                break;
            }
        }

        // Animate the piece falling into place.
        animate_falling_piece(mov_col, mov_row, 3 - get_to_move(frame));

    	// Check if someone has won.
        overall_winner = check_winner(frame);
        if (overall_winner != 0) {
            // Display the winner on the screen.
            display_winner(overall_winner);
            
            // Change the LED to blinking.
            led_blink();
            
            // Delay until reset.
            while (overall_winner != 0) {
                // Wait here for reset.
            }
            // Refresh the winner display.
            display_winner(0);
        }
        
    	// At this point we're ready to let the user make a move.
        mov_col = execute_user_move();
        
        // Animate the piece falling in.
        //display_frame(frame);
        // Figure out which row the piece was placed in so we
        // can animate it falling in.
        for (ridx = 5; ridx >= 0; ridx--) {
            if (get_at_col_row(frame, mov_col, ridx) != 0) {
                mov_row = ridx;
                break;
            }
        }
        animate_falling_piece(mov_col, mov_row, 3 - get_to_move(frame));
        
    	// Check if someone has won.
        overall_winner = check_winner(frame);
        if (overall_winner != 0) {
            display_winner(overall_winner);
            while (overall_winner != 0) {
                // Wait here for reset.
            }
            // Refresh the winner display.
            display_winner(0);
        }
	
    } // End loop that repeats each turn.
}