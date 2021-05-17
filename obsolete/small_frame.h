#ifndef SMALL_FRAME_H
#define SMALL_FRAME_H

struct FramePosition;

struct FramePosition* new_frame();

struct FramePosition* deep_copy_frame(struct FramePosition* template);

void free_frame(struct FramePosition* frame);

int get_to_move(struct FramePosition* frame);

int get_at_col_row(struct FramePosition* frame, int col, int row);

int move_in_col(struct FramePosition* frame, int col);

void cache_cells(struct FramePosition* frame);

void free_cell_cache(struct FramePosition* frame);

#endif
