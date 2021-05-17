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

void insert_piece_at_col_row(struct FramePosition* frame, int col, int row, char piece) {
    // Place a piece into the frame at the correct place.
    int i;
    
    struct NineByte addend;
    fill_nine_byte(&addend, (int) piece);

    // Multiply by 3 to shift it over to the correct cell.
    for (i = 0; i < 6*col + row; i++) {
	multiply_by_3(&addend);
    }

    // Add the addend in to insert the piece.
    add_nine_bytes(&(frame->position_rep), &addend);
}

void toggle_to_move(struct FramePosition* frame) {
    // Change whose move it is.
    frame->to_move = 3 - frame->to_move;
}

// Functions to manage cell caching.

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
