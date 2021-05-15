#ifndef FRAME_H
#define FRAME_H

struct FramePosition;

struct FramePosition* new_frame();

struct FramePosition* deep_copy_frame(struct FramePosition* template);

void free_frame(struct FramePosition* frame);

int get_to_move(struct FramePosition* frame);

int get_at_col_row(struct FramePosition* frame, int col, int row);

int move_in_col(struct FramePosition* frame, int col);
  
#endif
