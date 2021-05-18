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

void wait_for_user_move() {
    // Wait for the user's move (i.e., for user to press
    // one of the 7 column buttons), and then update the
    // frame accordingly.
    int move_rtn_val;
    
    while (1) {
        move_rtn_val = 1;
        
        if (Col_0_Read()) {
            move_rtn_val = move_in_col(frame, 0);
        } else if (Col_1_Read()) {
            move_rtn_val = move_in_col(frame, 1);
        } else if (Col_2_Read()) {
            move_rtn_val = move_in_col(frame, 2);
        } else if (Col_3_Read()) {
            move_rtn_val = move_in_col(frame, 3);
        } else if (Col_4_Read()) {
            move_rtn_val = move_in_col(frame, 4);
        } else if (Col_5_Read()) {
            move_rtn_val = move_in_col(frame, 5);
        } else if (Col_6_Read()) {
            move_rtn_val = move_in_col(frame, 6);
        }
        
        if (move_rtn_val == 0) {
            // We just executed a valid move.
            break;
        } else if (move_rtn_val == -1) {
            // The user input an invalid move.
            signal_illegal_move();
            move_rtn_val = 1;
        }
    }
}

int main() {
    // Start the clock/PWM hardware.
    Clock_1_Enable();
    Clock_2_Enable();
	PWM_1_Start();
	PWM_2_Start();
    
    // Set the LED to breathing slowly.
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
    	move_in_col(frame, get_best_move(root));

    	// Free the previous root game node (and with it the entire tree).
    	free_game_node(root);

    	// Display the new board position:
    	display_frame(frame);

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
        wait_for_user_move();
        
        // Display the frame again now.
        display_frame(frame);
        
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