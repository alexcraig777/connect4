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

int move_in_col(struct FramePosition* frame, int col) {
    // Update the supplied frame object with a new piece in
    // column col, if possible.
    // Return value of 0 indicates succes;
    // return value of -1 indicates move is illegal.
    int ret;

    if (frame->cells[col][5] == 0) {
        int row_to_check;
        for (row_to_check = 0; row_to_check < 6; row_to_check++) {
            if (frame->cells[col][row_to_check] == 0) {
                frame->cells[col][row_to_check] = frame->to_move;
                break;
            }
        }
        frame->to_move = 3 - frame->to_move;
        ret = 0;
    } else {
        ret = -1;
    }
    return ret;
}
