#ifndef FRAME_H
#define FRAME_H

struct FramePosition;

struct FramePosition* new_frame();

struct FramePosition* deep_copy_frame(struct FramePosition* template);

void free_frame(struct FramePosition* frame);

int get_to_move(struct FramePosition* frame);

int get_at_col_row(struct FramePosition* frame, int col, int row);


void insert_piece_at_col_row(struct FramePosition* frame,
			     int col, int row, char piece);

void toggle_to_move(struct FramePosition* frame);


void cache_cells(struct FramePosition* frame);

void free_cell_cache(struct FramePosition* frame);

#endif
