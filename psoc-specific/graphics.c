// This module is my wrapper over top of the TFT interface.
// It is the only module that should include tft.h.

#include "graphics.h"

#include "../frame.h"
#include "tft.h"

int color0 = 0x0000;
int color1 = 0x0afa;
int color2 = 0xffff;
int color3 = 0xf000;

void draw_cell(int ridx, int cidx, int player_idx) {
    int colors[4] = {color0, color1, color2, color3};
    
    uint16 SC, EC, SP, EP;

    EC = 240 - (18 + 30*cidx);
    SC = 240 - (42 + 30*cidx);
    
    SP = 3 + 30*ridx;
    EP = 27 + 30*ridx;
    
    write8_a0(0x2A);                 	// Column Address Set
    write8_a1(SC >> 8);                 // set SC[15:0]
    write8_a1(SC & 0x00FF);
    write8_a1(EC >> 8);                 // set EC[15:0]
    write8_a1(EC & 0x00FF);
    write8_a0(0x2B);                 	// Page Address Set
    write8_a1(SP >> 8);                 // set SP[15:0]
    write8_a1(SP & 0x00FF);
    write8_a1(EP >> 8);                 // set EP[15:0]
    write8_a1(EP & 0x00FF);
    write8_a0(0x2C);                    // Memory Write
    
    int i;
    for (i = 0; i < 625; i++) {
        write8_a1(colors[player_idx] >> 8);
        write8_a1(colors[player_idx] & 0xff);
    }
    write8_a0(0x00);                    // NOP
}

void display_frame(struct FramePosition* position) {
    int ridx;
    int cidx;

    // Tell the frame to cache its cells.
    cache_cells(position);
    
    for (ridx = 0; ridx < 6; ridx++) {
        for (cidx = 0; cidx < 7; cidx++) {
            draw_cell(ridx, cidx, get_at_col_row(position, cidx, ridx));
        }
    }
    // Tell the frame to free its cell cache.
    free_cell_cache(position);
}

void animate_falling_piece(int col, int row, int color) {
    // Animate the process of dropping the piece into the
    // board in a particular column and having it fall down
    // to the appropriate row.
    int current_row;
    for (current_row = 5; current_row > row; current_row--) {
        // Draw the piece in this row.
        draw_cell(current_row, col, color);
        CyDelay(50);
        // Erase the piece in this row.
        draw_cell(current_row, col, 0);
    }
    // Draw the piece in its final resting place.
    draw_cell(row, col, color);
}

void display_winner(int winner) {
    uint16 SC, EC, SP, EP;
    int i;
    
    int colors[4] = {color0, color1, color2, color3};

    
    SC = 18;
    EC = 240 - 18;
    
    SP = 30 * 7;
    EP = 300;

    write8_a0(0x2A);                 	// Column Address Set
    write8_a1(SC >> 8);                 // set SC[15:0]
    write8_a1(SC & 0x00FF);
    write8_a1(EC >> 8);                 // set EC[15:0]
    write8_a1(EC & 0x00FF);
    write8_a0(0x2B);                 	// Page Address Set
    write8_a1(SP >> 8);                 // set SP[15:0]
    write8_a1(SP & 0x00FF);
    write8_a1(EP >> 8);                 // set EP[15:0]
    write8_a1(EP & 0x00FF);
    write8_a0(0x2C);                    // Memory Write}
    
    for (i = 0; i < 21000; i++) {
        write8_a1(colors[winner] >> 8);
        write8_a1(colors[winner] & 0xff);
    }
    write8_a0(0x00);                    // NOP
}
