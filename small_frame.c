#include "small_frame.h"

#include "nine_byte.h"

#include <limits.h>
#include <stdlib.h>

#include <stdint.h>

// This file defines only the functions that need to access the internals
// of the frame structure. All other functions are defined in frame_ops.c.
// This will allow us to make the frame structure itself more efficient
// while changing the least code possible.

struct FramePosition {
    char to_move;
    struct NineByte position_rep;
};

struct FramePosition* new_frame() {
    // Allocate memory for a new frame structure, and
    // Intialize the memory to default values.
    struct FramePosition* frame;
    frame = malloc(sizeof(struct FramePosition));
    
    frame->to_move = 1;
    fill_nine_byte(&(frame->position_rep), 0);
    
    return frame;
}

struct FramePosition* deep_copy_frame(struct FramePosition* template) {
    // Allocate memory for a new frame structure, and
    // copy over all the data from the old frame.
    struct FramePosition* frame;
    frame = malloc(sizeof(struct FramePosition));
    
    frame->to_move = template->to_move;
    copy_nine_byte(&(frame->position_rep), &(template->position_rep));
    
    return frame;
}

void free_frame(struct FramePosition* frame) {
    // Release all memory currently used in this frame.
    // For this implementation this just means freeing the
    // frame structure itself.    
    free(frame);
}

int get_to_move(struct FramePosition* frame) {
    return frame->to_move;
}

int get_at_col_row(struct FramePosition* frame, int col, int row) {
    int idx;
    
    idx = 7*col + row;

    return get_trinary_digit(frame->position_rep, idx);
}

int move_in_col(struct FramePosition* frame, int col) {
    // Update the supplied frame object with a new piece in
    // column col, if possible.
    // Return value of 0 indicates succes;
    // return value of -1 indicates move is illegal.
    int rtn;
    int ridx;
    int idx, i;

    struct NineByte addend;
    
    rtn = -1;
    
    // Find the lowest cell in the column that is not occuppied.
    for (ridx = 0; ridx < 7; ridx++) {
	if (get_at_col_row(frame, col, ridx) == 0) {
	    // This space is empty; move here and return.
	    idx = 7*col + ridx;

	    fill_nine_byte(&addend, frame->to_move);
	    for (i = 0; i < idx; i++) {
		multiply_by_3(&addend);
	    }

	    add_nine_bytes(&(frame->position_rep), &addend);
	    
	    // Mark that we were able to make a valid move.
	    rtn = 0;
	    // Switch the person to move.
	    frame->to_move = 3 - frame->to_move;
	    break;
	}
    }
    return rtn;
}
