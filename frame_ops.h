#ifndef FRAME_OPS_H
#define FRAME_OPS_H

struct FramePosition;

int move_in_col(struct FramePosition* frame, int col);

int check_full(struct FramePosition* position);

int check_winner(struct FramePosition* position);

int score_frame(struct FramePosition* position, int player);

#endif
