#include "frame.h"

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
    char* cell_cache;
};

struct FramePosition* new_frame() {
    // Allocate memory for a new frame structure, and
    // Intialize the memory to default values.
    struct FramePosition* frame;
    frame = malloc(sizeof(struct FramePosition));
    
    frame->to_move = 1;
    fill_nine_byte(&(frame->position_rep), 0);
    frame->cell_cache = NULL;
    
    return frame;
}

struct FramePosition* deep_copy_frame(struct FramePosition* template) {
    // Allocate memory for a new frame structure, and
    // copy over all the data from the old frame.
    struct FramePosition* frame;
    frame = malloc(sizeof(struct FramePosition));
    
    frame->to_move = template->to_move;
    copy_nine_byte(&(frame->position_rep), &(template->position_rep));
    frame->cell_cache = NULL;
    
    return frame;
}

void free_frame(struct FramePosition* frame) {
    // Release all memory currently used in this frame.
    // For this implementation this just means freeing the
    // frame structure itself.
    // UPDATE: We really should also verify that the cell
    // cache is freed (if it's not already NULL).
    if (frame->cell_cache != NULL) {
	free(frame->cell_cache);
    }
    free(frame);
}

int get_to_move(struct FramePosition* frame) {
    return frame->to_move;
}

int get_at_col_row(struct FramePosition* frame, int col, int row) {
    // Check if the cells are cached. If they are, use the cache;
    // otherwise, refer to the representation.
    int idx;
    int rtn;

    if (frame->cell_cache != NULL) {
	// The cells are cached!
	rtn = frame->cell_cache[6*col + row];
    } else {
	idx = 6*col + row;
	rtn = get_trinary_digit(frame->position_rep, idx);
    }

    return rtn;
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
	    idx = 6*col + ridx;

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

void cache_cells(struct FramePosition* frame) {
    // Malloc and fill a cache containing the values in each cell.
    frame->cell_cache = malloc(42 * sizeof(char));

    get_trinary_digits(frame->position_rep, frame->cell_cache);
}

void free_cell_cache(struct FramePosition* frame) {
    // Free the cell cache and set it to NULL.
    free(frame->cell_cache);
    frame->cell_cache = NULL;
}
