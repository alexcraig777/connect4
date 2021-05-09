#include <project.h>
#include "tft.h"
#include "../frame.h"
#include "graphics.h"

#include <stdlib.h>

struct FramePosition* frame;
char winner;
int score;

CY_ISR( input_handler ) {
    int success;
    
    if (RST_BUTTON_Read()) {
        // Start over.
        free_frame(frame);
        frame = new_frame();
        winner = 0;
    } else if (winner == 0) {
        // Otherwise somebody is trying to make a move.
        if (Col_0_Read()) {
            success = move_in_col(frame, 0);
        } else if (Col_1_Read()) {
            success = move_in_col(frame, 1);
        } else if (Col_2_Read()) {
            success = move_in_col(frame, 2);
        } else if (Col_3_Read()) {
            success = move_in_col(frame, 3);
        } else if (Col_4_Read()) {
            success = move_in_col(frame, 4);
        } else if (Col_5_Read()) {
            success = move_in_col(frame, 5);
        } else if (Col_6_Read()) {
            success = move_in_col(frame, 6);
        }
    }
        
    // Check if the game is over.
    winner = check_winner(frame);
    
    // Display the frame.
    display_frame(frame);
    
    score = score_frame(frame, 1);
}

int main()
{
    // Enable global interrupts.
    CyGlobalIntEnable;
    
    // Initialize SPIM component and the TFT display.
    SPIM_1_Start();
    tftStart();
    
    frame = new_frame();
    
    display_frame(frame);
    
    display_winner(0);
    
    // Initialize the interrupt from the user buttons.
    USER_INPUT_StartEx(input_handler);
    
    while (1) {
        if (winner != 0) {
            display_winner(winner);
            while (winner != 0) {
                // Wait here for reset.
            }
            // Refresh the winner display.
            display_winner(0);
        }
    }

}

/* [] END OF FILE */
