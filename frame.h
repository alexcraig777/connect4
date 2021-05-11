#ifndef FRAME_H
#define FRAME_H

struct FramePosition {
    char to_move;
    char **cells;
};

struct FramePosition* new_frame();

struct FramePosition* deep_copy_frame(struct FramePosition* template);

void free_frame(struct FramePosition* frame);

int move_in_col(struct FramePosition* frame, int col);

void display_frame(struct FramePosition* position);

int check_full(struct FramePosition* position);

int check_winner(struct FramePosition* position);

int score_frame(struct FramePosition* position, int player);

#endif
