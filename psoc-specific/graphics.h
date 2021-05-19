#ifndef GRAPHICS_H
#define GRAPHICS_H
    
#include "../frame.h"

void draw_cell(int ridx, int cidx, int color);

void display_frame(struct FramePosition* position);

void animate_falling_piece(int col, int row, int player_idx);

void display_winner(int winner);

#endif