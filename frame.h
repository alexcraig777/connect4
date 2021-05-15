#ifndef FRAME_H
#define FRAME_H

struct FramePosition* new_frame();

struct FramePosition* deep_copy_frame(struct FramePosition* template);

void free_frame(struct FramePosition* frame);


int get_to_move(struct FramePosition* frame);

int get_at_col_row(struct FramePosition* frame, int col, int row);

int move_in_col(struct FramePosition* frame, int col);

void display_frame(struct FramePosition* position);

int check_full(struct FramePosition* position);

int check_winner(struct FramePosition* position);

int score_frame(struct FramePosition* position, int player);

#endif
