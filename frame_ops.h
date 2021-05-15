#ifndef FRAME_OPS_H
#define FRAME_OPS_H

struct FramePosition;

void display_frame(struct FramePosition* position);

int check_full(struct FramePosition* position);

int check_winner(struct FramePosition* position);

int score_frame(struct FramePosition* position, int player);

#endif
