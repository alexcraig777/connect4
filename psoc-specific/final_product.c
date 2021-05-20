#include <project.h>
#include "tft.h"
#include "graphics.h"
#include "led_control.h"

#include "../frame.h"
#include "../frame_ops.h"
#include "../game_node.h"

#include <stdlib.h>

#define COMPUTER 0
#define HUMAN 1

#define MAX_EXPANSIONS 70

struct FramePosition* frame;
char overall_winner;
int score;


void draw_last_move(int mov_col) {
    // Figure out which row the piece was placed in,
    // so that we can animate the piece falling in.
    int mov_row;
    mov_row = 0;
    
    int ridx;
    for (ridx = 5; ridx >= 0; ridx--) {
        if (get_at_col_row(frame, mov_col, ridx) != 0) {
            mov_row = ridx;
            break;
        }
    }
    // Animate the piece falling into place.
    animate_falling_piece(mov_col, mov_row, 3 - get_to_move(frame));
}

int execute_user_move() {
    // Wait for the user's move (i.e., for user to press
    // one of the 7 column buttons), and then update the
    // frame accordingly.
    // Return the column in which the player moved.
    int mov_col;
    int move_rtn_val;
    
    while (1) {
        mov_col = 8;
        if (Col_0_Read()) {
            mov_col = 0;
        } else if (Col_1_Read()) {
            mov_col = 1;
        } else if (Col_2_Read()) {
            mov_col = 2;
        } else if (Col_3_Read()) {
            mov_col = 3;
        } else if (Col_4_Read()) {
            mov_col = 4;
        } else if (Col_5_Read()) {
            mov_col = 5;
        } else if (Col_6_Read()) {
            mov_col = 6;
        } else if (User_reset_Read()) {
            mov_col = 7;
        }
        
        if (mov_col != 8) {
            move_rtn_val = move_in_col(frame, mov_col);
            if (move_rtn_val == 0) {
                // We just execited a valid move.
                break;
            } else {
                // The user input an invalid move.
                signal_illegal_move();
            }
        }
    }

    // As long as the user didn't tell us to reset,
    // draw the move.
    if (mov_col != 7) {
        draw_last_move(mov_col);
    }
    
    return mov_col;
}

void make_psoc_move() {
    struct GameNode* root;
    struct GameNode* leaf;
    
    char winner;
    
    int mov_col;
    
	// Initialize a game node for the current position.
	root = new_game_node(NULL, frame);

	// Repeatedly search the tree descending from root.
    int search_counter;
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

    // Draw the move.
    draw_last_move(mov_col);
}

int evaluate_game_state() {
    // Check if someone has won or if the game is tied.
    // If not, just return with value 0.
    // Otherwise, stop the game, update display and LED as
    // necessary, and wait for the user to reset. Then return
    // with value 0.
    int rst_on_rtn;
    rst_on_rtn = 0;
    
    int winning_row_indices[4], winning_col_indices[4];

	// Check if someone has won.
    overall_winner = check_winner(frame);
    if (overall_winner != 0) {
        // Mark that we need to reset when we return from this.
        rst_on_rtn = 1;
        
        // Display the winner and change the LED to blinking.
        display_winner(overall_winner);
        led_blink();

        // Figure out where they won so that we can make the winning
        // pieces blink.
        find_winning_indices(frame, winning_row_indices, winning_col_indices);
        while (overall_winner != 0) {
            // Wait here for reset, blinking the pieces while we wait.
            int i;
            CyDelay(500);
            for (i = 0; i < 4; i++) {
                // Fill in the cells with red.
                draw_cell(winning_row_indices[i],
                          winning_col_indices[i],
                          3);
            }
            // Check if the user has asked us to reset.
            if (User_reset_Read()) {
                break;
            }
            CyDelay(500);
            for (i = 0; i < 4; i++) {
                // Fill in the cells with the original colors.
                draw_cell(winning_row_indices[i],
                          winning_col_indices[i],
                          overall_winner);
            }
            
            // Check if the user has asked us to reset.
            if (User_reset_Read()) {
                break;
            }
        }
    } else if (check_full(frame) != 0) {
        // The game is tied! Who'd have thought!
        // I guess just set the light to blinking and wait
        // for the user to reset.
        led_blink();
        rst_on_rtn = 1;
        while (1) {
            if (User_reset_Read()) {
                break;
            }
        }
    }
    
    return rst_on_rtn;
}

void play_game(char player_1, char player_2) {
    int user_move;
    int rst;
    
    while (1) {
        if (player_1 == COMPUTER) {
            make_psoc_move();
        } else {
            user_move = execute_user_move();
            if (user_move == 7) {
                // The user ordered a reset.
                break;
            }
        }
        rst = evaluate_game_state();
        if (rst) {
            break;
        }
        
        if (player_2 == COMPUTER) {
            make_psoc_move();
        } else {
            user_move = execute_user_move();
            if (user_move == 7) {
                // The user ordered a reset.
                break;
            }
        }
        rst = evaluate_game_state();
        if (rst) {
            break;
        }
    }
    return;
}

int get_game_mode() {
    // Before we play a game, the user presses a column button
    // to select one of 3 game play modes:
    //   rtn = 0 => psoc vs. human
    //   rtn = 1 => human vs. psoc
    //   rtn = 2 => human vs. human
    //   rtn = 3 => psoc vs. psoc
    
    // Turn the LED constant on to indicate we're on this state.
    led_on();
    
    int rtn;
    rtn = 0;
    
    while (1) {
        if (Col_0_Read()) {
            rtn = 0;
            break;
        }
        if (Col_1_Read()) {
            rtn = 1;
            // This is kind of stupid, but delaying for a second is the
            // easiest way to make sure that the user's key press is
            // not immediately interpreted as her first move.
            CyDelay(1000);
            break;
        }
        if (Col_2_Read()) {
            rtn = 2;
            // Same story as above.
            CyDelay(1000);
            break;
        }
        if (Col_3_Read()) {
            rtn = 3;
            break;
        }
    }
    
    // Make the LED breate to indicate the game is beginning.
    led_breathe();
    
    return rtn;
}

int main() {
    // Start the clock/PWM hardware.
    Clock_1_Enable();
    Clock_2_Enable();
	PWM_1_Start();
	PWM_2_Start();
    
    // Start the LED on.
    led_on();
    
    // Enable global interrupts.
    CyGlobalIntEnable;
    
    // Initialize SPIM component and the TFT display.
    SPIM_1_Start();
    tftStart();
        
    // Initialize the interrupt from the user buttons.
    //USER_INPUT_StartEx(input_handler);
    
    int game_mode, player_1, player_2;
    while (1) {
        // Reset the frame and the display.
        frame = new_frame();
        display_frame(frame);
        display_winner(0);

        // Have the user input a mode.
        game_mode = get_game_mode();
        
        // Interpret the game mode as a set of players.
        if ((game_mode == 0) || (game_mode == 3)) {
            player_1 = COMPUTER;
        } else {
            player_1 = HUMAN;
        }
        if ((game_mode == 1) || (game_mode == 3)) {
            player_2 = COMPUTER;
        } else {
            player_2 = HUMAN;
        }
        // Play the game!
        play_game(player_1, player_2);
        
        free_frame(frame);
    }
}