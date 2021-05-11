#include "frame.h"

#include <limits.h>
#include <stdlib.h>

#define WINNING_SCORE 10000


struct FramePosition* new_frame() {
    // Allocate memory for a new frame structure, and
    // intialize the memory to default values.
    int ridx, cidx;
    
    char **cells = malloc(7 * sizeof(char*));
    for (cidx = 0; cidx < 7; cidx++) {
        cells[cidx] = malloc(6 * sizeof(char));
        for (ridx = 0; ridx < 6; ridx++) {
            cells[cidx][ridx] = 0;
        }
    }
    
    struct FramePosition* frame;
    frame = malloc(sizeof(struct FramePosition));
    
    frame->cells = cells;
    frame->to_move = 1;
    
    return frame;
}

struct FramePosition* deep_copy_frame(struct FramePosition* template) {
    // Allocate memory for a new frame structure, and
    // copy over all the data from the old frame.
    int ridx, cidx;
    
    char **cells = malloc(7 * sizeof(char*));
    for (cidx = 0; cidx < 7; cidx++) {
        cells[cidx] = malloc(6 * sizeof(char));
        for (ridx = 0; ridx < 6; ridx++) {
            cells[cidx][ridx] = template->cells[cidx][ridx];
        }
    }
    
    struct FramePosition* frame;
    frame = malloc(sizeof(struct FramePosition));
    
    frame->cells = cells;
    frame->to_move = template->to_move;
    
    return frame;
}

void free_frame(struct FramePosition* frame) {
    // Release all memory currently used in this frame.
    // We need to make sure we get each column, the column pointers, and
    // the frame itself.
    int col;
    
    for (col = 0; col < 7; col++) {
        free(frame->cells[col]);
    }
    
    free(frame->cells);
    
    free(frame);
}

int move_in_col(struct FramePosition* frame, int col) {
    // Update the supplied frame object with a new piece in
    // column col, if possible.
    // Return value of 0 indicates succes;
    // return value of -1 indicates move is illegal.
    int ret;
    
    if (frame->cells[col][5] == 0) {
        int row_to_check;
        for (row_to_check = 0; row_to_check < 6; row_to_check++) {
            if (frame->cells[col][row_to_check] == 0) {
                frame->cells[col][row_to_check] = frame->to_move;
                break;
            }
        }
        frame->to_move = 3 - frame->to_move;
        ret = 0;
    } else {
        ret = -1;
    }
    return ret;
}

int check_winner(struct FramePosition* position) {
    // Check if the game is 
    int cidx, ridx;
    int base_ridx, base_cidx;
    char winner;
    winner = 0;

    // Check all columns for 4-in-a-row.
    for (cidx = 0; cidx < 7; cidx++) {
        for (base_ridx = 0; base_ridx < 3; base_ridx++) {
            if ((position->cells[cidx][base_ridx] == 1) &&
            (position->cells[cidx][base_ridx + 1] == 1) &&
            (position->cells[cidx][base_ridx + 2] == 1) &&
            (position->cells[cidx][base_ridx + 3] == 1)) {
                winner = 1;
                break;
            } else if ((position->cells[cidx][base_ridx] == 2) &&
            (position->cells[cidx][base_ridx + 1] == 2) &&
            (position->cells[cidx][base_ridx + 2] == 2) &&
            (position->cells[cidx][base_ridx + 3] == 2)) {
                winner = 2;
                break;
            }
        }
        if (winner != 0) {
            break;
        }
    }
    
    // Check all rows for 4-in-a-row.
    if (winner == 0) {
        for (ridx = 0; ridx < 6; ridx++) {
            for (base_cidx = 0; base_cidx < 4; base_cidx++) {
                if ((position->cells[base_cidx][ridx] == 1) &&
                (position->cells[base_cidx + 1][ridx] == 1) &&
                (position->cells[base_cidx + 2][ridx] == 1) &&
                (position->cells[base_cidx + 3][ridx] == 1)) {
                    winner = 1;
                    break;
                } else if ((position->cells[base_cidx][ridx] == 2) &&
                (position->cells[base_cidx + 1][ridx] == 2) &&
                (position->cells[base_cidx + 2][ridx] == 2) &&
                (position->cells[base_cidx + 3][ridx] == 2)) {
                    winner = 2;
                    break;
                }
            }
            if (winner != 0) {
                break;
            }
        }
    }
    
    // Check all SW-NE diagonals for 4-in-a-row.
    if (winner == 0) {
        for (base_cidx = 0; base_cidx < 4; base_cidx++) {
            for (base_ridx = 0; base_ridx < 3; base_ridx++) {
                if ((position->cells[base_cidx][base_ridx] == 1) &&
                (position->cells[base_cidx + 1][base_ridx + 1] == 1) &&
                (position->cells[base_cidx + 2][base_ridx + 2] == 1) &&
                (position->cells[base_cidx + 3][base_ridx + 3] == 1)) {
                    winner = 1;
                    break;
                } else if ((position->cells[base_cidx][base_ridx] == 2) &&
                (position->cells[base_cidx + 1][base_ridx + 1] == 2) &&
                (position->cells[base_cidx + 2][base_ridx + 2] == 2) &&
                (position->cells[base_cidx + 3][base_ridx + 3] == 2)) {
                    winner = 2;
                    break;
                }
            }
        }
    }

    // Check all NW-SE diagonals for 4-in-a-row.
    if (winner == 0) {
        for (base_cidx = 0; base_cidx < 4; base_cidx++) {
            for (base_ridx = 3; base_ridx < 6; base_ridx++) {
                if ((position->cells[base_cidx][base_ridx] == 1) &&
                (position->cells[base_cidx + 1][base_ridx - 1] == 1) &&
                (position->cells[base_cidx + 2][base_ridx - 2] == 1) &&
                (position->cells[base_cidx + 3][base_ridx - 3] == 1)) {
                    winner = 1;
                    break;
                } else if ((position->cells[base_cidx][base_ridx] == 2) &&
                (position->cells[base_cidx + 1][base_ridx - 1] == 2) &&
                (position->cells[base_cidx + 2][base_ridx - 2] == 2) &&
                (position->cells[base_cidx + 3][base_ridx - 3] == 2)) {
                    winner = 2;
                    break;
                }
            }
        }
    }
    
    return winner;
}

int score_frame(struct FramePosition* position, int player) {
    int cidx, ridx;
    int base_ridx, base_cidx;
    int score;
    score = 0;

    char winner = 0;
    char contiguous_pieces = 0;
    
    int values[5] = {0, 1, 3, 5, 0};
    
    // Check all columns for points for both players.
    for (cidx = 0; cidx < 7; cidx++) {
        for (base_ridx = 0; base_ridx < 3; base_ridx++) {
            // If player 2 has no pieces in this chunk, then count how
            // many pieces player 1 has in this chunk.
            if ((position->cells[cidx][base_ridx] != 2) &&
            (position->cells[cidx][base_ridx + 1] != 2) &&
            (position->cells[cidx][base_ridx + 2] != 2) &&
            (position->cells[cidx][base_ridx + 3] != 2)) {
                contiguous_pieces = ((position->cells[cidx][base_ridx] == 1) +
                                     (position->cells[cidx][base_ridx + 1] == 1) +
                                     (position->cells[cidx][base_ridx + 2] == 1) +
                                     (position->cells[cidx][base_ridx + 3] == 1));
                if (player == 1) {
                    score += values[(int) contiguous_pieces];
                } else {
                    score -= values[(int) contiguous_pieces];
                }
		if (contiguous_pieces == 4) {
		    winner = 1;
		}
            // Otherise, if player 1 has no pieces in this chunk,
            // then count how many pieces player 2 has in this chunk.
            } else if ((position->cells[cidx][base_ridx] != 1) &&
            (position->cells[cidx][base_ridx + 1] != 1) &&
            (position->cells[cidx][base_ridx + 2] != 1) &&
            (position->cells[cidx][base_ridx + 3] != 1)) {
                contiguous_pieces = ((position->cells[cidx][base_ridx] == 2) +
                                     (position->cells[cidx][base_ridx + 1] == 2) +
                                     (position->cells[cidx][base_ridx + 2] == 2) +
                                     (position->cells[cidx][base_ridx + 3] == 2));
                if (player == 2) {
                    score += values[(int) contiguous_pieces];
                } else {
                    score -= values[(int) contiguous_pieces];
                }
		if (contiguous_pieces == 4) {
		    winner = 2;
		}
            }
        }
    }
    
    // Check all rows for points for both players.
    for (ridx = 0; ridx < 6; ridx++) {
        for (base_cidx = 0; base_cidx < 4; base_cidx++) {
            // If player 2 has no pieces in this chunk, then count how
            // many pieces player 1 has in this chunk.
            if ((position->cells[base_cidx][ridx] != 2) &&
            (position->cells[base_cidx + 1][ridx] != 2) &&
            (position->cells[base_cidx + 2][ridx] != 2) &&
            (position->cells[base_cidx + 3][ridx] != 2)) {
                contiguous_pieces = ((position->cells[base_cidx][ridx] == 1) +
                                     (position->cells[base_cidx + 1][ridx] == 1) +
                                     (position->cells[base_cidx + 2][ridx] == 1) +
                                     (position->cells[base_cidx + 3][ridx] == 1));
                if (player == 1) {
                    score += values[(int) contiguous_pieces];
                } else {
                    score -= values[(int) contiguous_pieces];
                }
		if (contiguous_pieces == 4) {
		    winner = 1;
		}
            // Otherise, if player 1 has no pieces in this chunk,
            // then count how many pieces player 2 has in this chunk.
            } else if ((position->cells[base_cidx][ridx] != 1) &&
            (position->cells[base_cidx + 1][ridx] != 1) &&
            (position->cells[base_cidx + 2][ridx] != 1) &&
            (position->cells[base_cidx + 3][ridx] != 1)) {
                contiguous_pieces = ((position->cells[base_cidx][ridx] == 2) +
                                     (position->cells[base_cidx + 1][ridx] == 2) +
                                     (position->cells[base_cidx + 2][ridx] == 2) +
                                     (position->cells[base_cidx + 3][ridx] == 2));
                if (player == 2) {
                    score += values[(int) contiguous_pieces];
                } else {
                    score -= values[(int) contiguous_pieces];
                }
		if (contiguous_pieces == 4) {
		    winner = 2;
		}
            }
        }
    }

    // Check all SW-NE diagonals for scores for both players.
    for (base_cidx = 0; base_cidx < 4; base_cidx++) {
        for (base_ridx = 0; base_ridx < 3; base_ridx++) {
            // If player 2 has no pieces in this block, count player 1's
            // score.
            if ((position->cells[base_cidx][base_ridx] != 2) &&
            (position->cells[base_cidx + 1][base_ridx + 1] != 2) &&
            (position->cells[base_cidx + 2][base_ridx + 2] != 2) &&
            (position->cells[base_cidx + 3][base_ridx + 3] != 2)) {
                contiguous_pieces = ((position->cells[base_cidx][base_ridx] == 1) +
                                     (position->cells[base_cidx + 1][base_ridx + 1] == 1) +
                                     (position->cells[base_cidx + 2][base_ridx + 2] == 1) +
                                     (position->cells[base_cidx + 3][base_ridx + 3] == 1));
                if (player == 1) {
                    score += values[(int) contiguous_pieces];
                } else {
                    score -= values[(int) contiguous_pieces];
                }
		if (contiguous_pieces == 4) {
		    winner = 1;
		}
            // Otherwise, if player 1 has no pieces in this block, count
            // player 2's score.
            } else if ((position->cells[base_cidx][base_ridx] != 1) &&
            (position->cells[base_cidx + 1][base_ridx + 1] != 1) &&
            (position->cells[base_cidx + 2][base_ridx + 2] != 1) &&
            (position->cells[base_cidx + 3][base_ridx + 3] != 1)) {
                contiguous_pieces = ((position->cells[base_cidx][base_ridx] == 2) +
                                     (position->cells[base_cidx + 1][base_ridx + 1] == 2) +
                                     (position->cells[base_cidx + 2][base_ridx + 2] == 2) +
                                     (position->cells[base_cidx + 3][base_ridx + 3] == 2));
                if (player == 2) {
                    score += values[(int) contiguous_pieces];
                } else {
                    score -= values[(int) contiguous_pieces];
                }
		if (contiguous_pieces == 4) {
		    winner = 2;
		}
            }
        }
    }
    
    // Check all NW-SE diagonals for scores for both players.
    for (base_cidx = 0; base_cidx < 4; base_cidx++) {
        for (base_ridx = 3; base_ridx < 6; base_ridx++) {
            // If player 2 has no pieces in this block, count player 1's
            // score.
            if ((position->cells[base_cidx][base_ridx] != 2) &&
            (position->cells[base_cidx + 1][base_ridx - 1] != 2) &&
            (position->cells[base_cidx + 2][base_ridx - 2] != 2) &&
            (position->cells[base_cidx + 3][base_ridx - 3] != 2)) {
                contiguous_pieces = ((position->cells[base_cidx][base_ridx] == 1) +
                                     (position->cells[base_cidx + 1][base_ridx - 1] == 1) +
                                     (position->cells[base_cidx + 2][base_ridx - 2] == 1) +
                                     (position->cells[base_cidx + 3][base_ridx - 3] == 1));
                if (player == 1) {
                    score += values[(int) contiguous_pieces];
                } else {
                    score -= values[(int) contiguous_pieces];
                }
		if (contiguous_pieces == 4) {
		    winner = 1;
		}
            // Otherwise, if player 1 has no pieces in this block, count
            // player 2's score.
            } else if ((position->cells[base_cidx][base_ridx] != 1) &&
            (position->cells[base_cidx + 1][base_ridx - 1] != 1) &&
            (position->cells[base_cidx + 2][base_ridx - 2] != 1) &&
            (position->cells[base_cidx + 3][base_ridx - 3] != 1)) {
                contiguous_pieces = ((position->cells[base_cidx][base_ridx] == 2) +
                                     (position->cells[base_cidx + 1][base_ridx - 1] == 2) +
                                     (position->cells[base_cidx + 2][base_ridx - 2] == 2) +
                                     (position->cells[base_cidx + 3][base_ridx - 3] == 2));
                if (player == 2) {
                    score += values[(int) contiguous_pieces];
                } else {
                    score -= values[(int) contiguous_pieces];
                }
		if (contiguous_pieces == 4) {
		    winner = 2;
		}
            }
        }
    }

    if (winner != 0) {
	// Somebody won. The score we should return is some maximum minus
	// the number of moves it took them to get there.
	int num_moves = 0;
	for (cidx = 0; cidx < 7; cidx++) {
	    for (ridx = 0; ridx < 6; ridx++) {
		if (position->cells[cidx][ridx] != 0) {
		    num_moves++;
		}
	    }
	}
	if (winner == 1) {
	    score = WINNING_SCORE - num_moves;
	} else {
	    score = -WINNING_SCORE + num_moves;
	}
    }
    return score;
}
