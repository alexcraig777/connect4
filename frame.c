#include "frame.h"

#include <limits.h>
#include <stdlib.h>

// This file defines only the functions that need to access the internals
// of the frame structure. All other functions are defined in frame_ops.c.
// This will allow us to make the frame structure itself more efficient
// while changing the least code possible.

struct FramePosition {
    char to_move;
    char **cells;
};

struct FramePosition* new_frame() {
    // Allocate memory for a new frame structure, and
    // intialize the memory to default values.
    int ridx, cidx;
    
    char **cells = malloc(7 * sizeof(char*));
    for (cidx = 0; cidx < 7; cidx++) {
        cells[cidx] = malloc(6 * sizeof(char));
        for (ridx = 0; ridx < 6; ridx++) {
            cells[cidx][ridx] = 0;
        }
    }
    
    struct FramePosition* frame;
    frame = malloc(sizeof(struct FramePosition));
    
    frame->cells = cells;
    frame->to_move = 1;
    
    return frame;
}

struct FramePosition* deep_copy_frame(struct FramePosition* template) {
    // Allocate memory for a new frame structure, and
    // copy over all the data from the old frame.
    int ridx, cidx;
    
    char **cells = malloc(7 * sizeof(char*));
    for (cidx = 0; cidx < 7; cidx++) {
        cells[cidx] = malloc(6 * sizeof(char));
        for (ridx = 0; ridx < 6; ridx++) {
            cells[cidx][ridx] = template->cells[cidx][ridx];
        }
    }
    
    struct FramePosition* frame;
    frame = malloc(sizeof(struct FramePosition));
    
    frame->cells = cells;
    frame->to_move = template->to_move;
    
    return frame;
}

void free_frame(struct FramePosition* frame) {
    // Release all memory currently used in this frame.
    // We need to make sure we get each column, the column pointers, and
    // the frame itself.
    int col;
    
    for (col = 0; col < 7; col++) {
        free(frame->cells[col]);
    }
    
    free(frame->cells);
    
    free(frame);
}

int get_to_move(struct FramePosition* frame) {
    return frame->to_move;
}

int get_at_col_row(struct FramePosition* frame, int col, int row) {
    return (int) frame->cells[col][row];
}

void insert_piece_at_col_row(struct FramePosition* frame, int col, int row, char piece) {
    // Place a piece into the frame at the correct place.
    frame->cells[col][row] = piece;
}

void toggle_to_move(struct FramePosition* frame) {
    // Change whose move it is.
    frame->to_move = 3 - frame->to_move;
}

// Functions to manage cell caching.

void cache_cells(struct FramePosition* frame) {
    // We just need this to be defined to keep the frame
    // exterior interface consistend.
}

void free_cell_cache(struct FramePosition* frame) {
    // We just need this to be defined to keep the frame
    // exterior interface consistend.
}
