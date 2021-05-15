#include "frame_ops.h"
#include "frame.h"

#include <limits.h>
#include <stdlib.h>

#define WINNING_SCORE 10000

// This file defines all the operations on frame structures
// that do not directly depend on the internals of the structure.
// Separating them out will allow us more easily to make the
// structure more space-efficient.

int check_full(struct FramePosition* position) {
    // Check if the board is completely full. This does not check
    // if anyone has won the game.
    int cidx;
    int rtn;

    rtn = 1;
    for (cidx = 0; cidx < 7; cidx++) {
        if (get_at_col_row(position, cidx, 5) == 0) {
            // There is an empty cell. The game is not over.
            // We can immediately return 0.
            rtn = 0;
            break;
        }
    }
    // If we get here without ever finding an empty cell, then the
    // board is full and we should return 1.
    return rtn;
}

int check_winner(struct FramePosition* position) {
    // Check if either player has won. This does not check if the
    // board is full (i.e., the game is a tie).
    int cidx, ridx;
    int base_ridx, base_cidx;
    char winner;
    winner = 0;

    // Check all columns for 4-in-a-row.
    for (cidx = 0; cidx < 7; cidx++) {
        for (base_ridx = 0; base_ridx < 3; base_ridx++) {
            if ((get_at_col_row(position, cidx, base_ridx) == 1) &&
		(get_at_col_row(position, cidx, base_ridx + 1) == 1) &&
		(get_at_col_row(position, cidx, base_ridx + 1) == 1) &&
		(get_at_col_row(position, cidx, base_ridx + 1) == 1)) {
                winner = 1;
                break;
            } else if ((get_at_col_row(position, cidx, base_ridx) == 2) &&
		       (get_at_col_row(position, cidx, base_ridx + 1) == 2) &&
		       (get_at_col_row(position, cidx, base_ridx + 2) == 2) &&
		       (get_at_col_row(position, cidx, base_ridx + 3) == 2)) {
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
                if ((get_at_col_row(position, base_cidx, ridx) == 1) &&
                (get_at_col_row(position, base_cidx + 1, ridx) == 1) &&
                (get_at_col_row(position, base_cidx + 2, ridx) == 1) &&
                (get_at_col_row(position, base_cidx + 3, ridx) == 1)) {
                    winner = 1;
                    break;
                } else if ((get_at_col_row(position, base_cidx, ridx) == 2) &&
                (get_at_col_row(position, base_cidx + 1, ridx) == 2) &&
                (get_at_col_row(position, base_cidx + 2, ridx) == 2) &&
                (get_at_col_row(position, base_cidx + 3, ridx) == 2)) {
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
                if ((get_at_col_row(position, base_cidx, base_ridx) == 1) &&
                (get_at_col_row(position, base_cidx + 1, base_ridx + 1) == 1) &&
                (get_at_col_row(position, base_cidx + 2, base_ridx + 2) == 1) &&
                (get_at_col_row(position, base_cidx + 3, base_ridx + 3) == 1)) {
                    winner = 1;
                    break;
                } else if ((get_at_col_row(position, base_cidx, base_ridx) == 2) &&
                (get_at_col_row(position, base_cidx + 1, base_ridx + 1) == 2) &&
                (get_at_col_row(position, base_cidx + 2, base_ridx + 2) == 2) &&
                (get_at_col_row(position, base_cidx + 3, base_ridx + 3) == 2)) {
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
                if ((get_at_col_row(position, base_cidx, base_ridx) == 1) &&
                (get_at_col_row(position, base_cidx + 1, base_ridx - 1) == 1) &&
                (get_at_col_row(position, base_cidx + 2, base_ridx - 2) == 1) &&
                (get_at_col_row(position, base_cidx + 3, base_ridx - 3) == 1)) {
                    winner = 1;
                    break;
                } else if ((get_at_col_row(position, base_cidx, base_ridx) == 2) &&
                (get_at_col_row(position, base_cidx + 1, base_ridx - 1) == 2) &&
                (get_at_col_row(position, base_cidx + 2, base_ridx - 2) == 2) &&
                (get_at_col_row(position, base_cidx + 3, base_ridx - 3) == 2)) {
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
            if ((get_at_col_row(position, cidx, base_ridx) != 2) &&
            (get_at_col_row(position, cidx, base_ridx + 1) != 2) &&
            (get_at_col_row(position, cidx, base_ridx + 2) != 2) &&
            (get_at_col_row(position, cidx, base_ridx + 3) != 2)) {
                contiguous_pieces = ((get_at_col_row(position, cidx, base_ridx) == 1) +
                                     (get_at_col_row(position, cidx, base_ridx + 1) == 1) +
                                     (get_at_col_row(position, cidx, base_ridx + 2) == 1) +
                                     (get_at_col_row(position, cidx, base_ridx + 3) == 1));
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
            } else if ((get_at_col_row(position, cidx, base_ridx) != 1) &&
            (get_at_col_row(position, cidx, base_ridx + 1) != 1) &&
            (get_at_col_row(position, cidx, base_ridx + 2) != 1) &&
            (get_at_col_row(position, cidx, base_ridx + 3) != 1)) {
                contiguous_pieces = ((get_at_col_row(position, cidx, base_ridx) == 2) +
                                     (get_at_col_row(position, cidx, base_ridx + 1) == 2) +
                                     (get_at_col_row(position, cidx, base_ridx + 2) == 2) +
                                     (get_at_col_row(position, cidx, base_ridx + 3) == 2));
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
            if ((get_at_col_row(position, base_cidx, ridx) != 2) &&
            (get_at_col_row(position, base_cidx + 1, ridx) != 2) &&
            (get_at_col_row(position, base_cidx + 2, ridx) != 2) &&
            (get_at_col_row(position, base_cidx + 3, ridx) != 2)) {
                contiguous_pieces = ((get_at_col_row(position, base_cidx, ridx) == 1) +
                                     (get_at_col_row(position, base_cidx + 1, ridx) == 1) +
                                     (get_at_col_row(position, base_cidx + 2, ridx) == 1) +
                                     (get_at_col_row(position, base_cidx + 3, ridx) == 1));
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
            } else if ((get_at_col_row(position, base_cidx, ridx) != 1) &&
            (get_at_col_row(position, base_cidx + 1, ridx) != 1) &&
            (get_at_col_row(position, base_cidx + 2, ridx) != 1) &&
            (get_at_col_row(position, base_cidx + 3, ridx) != 1)) {
                contiguous_pieces = ((get_at_col_row(position, base_cidx, ridx) == 2) +
                                     (get_at_col_row(position, base_cidx + 1, ridx) == 2) +
                                     (get_at_col_row(position, base_cidx + 2, ridx) == 2) +
                                     (get_at_col_row(position, base_cidx + 3, ridx) == 2));
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
            if ((get_at_col_row(position, base_cidx, base_ridx) != 2) &&
            (get_at_col_row(position, base_cidx + 1, base_ridx + 1) != 2) &&
            (get_at_col_row(position, base_cidx + 2, base_ridx + 2) != 2) &&
            (get_at_col_row(position, base_cidx + 3, base_ridx + 3) != 2)) {
                contiguous_pieces = ((get_at_col_row(position, base_cidx, base_ridx) == 1) +
                                     (get_at_col_row(position, base_cidx + 1, base_ridx + 1) == 1) +
                                     (get_at_col_row(position, base_cidx + 2, base_ridx + 2) == 1) +
                                     (get_at_col_row(position, base_cidx + 3, base_ridx + 3) == 1));
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
            } else if ((get_at_col_row(position, base_cidx, base_ridx) != 1) &&
            (get_at_col_row(position, base_cidx + 1, base_ridx + 1) != 1) &&
            (get_at_col_row(position, base_cidx + 2, base_ridx + 2) != 1) &&
            (get_at_col_row(position, base_cidx + 3, base_ridx + 3) != 1)) {
                contiguous_pieces = ((get_at_col_row(position, base_cidx, base_ridx) == 2) +
                                     (get_at_col_row(position, base_cidx + 1, base_ridx + 1) == 2) +
                                     (get_at_col_row(position, base_cidx + 2, base_ridx + 2) == 2) +
                                     (get_at_col_row(position, base_cidx + 3, base_ridx + 3) == 2));
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
            if ((get_at_col_row(position, base_cidx, base_ridx) != 2) &&
            (get_at_col_row(position, base_cidx + 1, base_ridx - 1) != 2) &&
            (get_at_col_row(position, base_cidx + 2, base_ridx - 2) != 2) &&
            (get_at_col_row(position, base_cidx + 3, base_ridx - 3) != 2)) {
                contiguous_pieces = ((get_at_col_row(position, base_cidx, base_ridx) == 1) +
                                     (get_at_col_row(position, base_cidx + 1, base_ridx - 1) == 1) +
                                     (get_at_col_row(position, base_cidx + 2, base_ridx - 2) == 1) +
                                     (get_at_col_row(position, base_cidx + 3, base_ridx - 3) == 1));
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
            } else if ((get_at_col_row(position, base_cidx, base_ridx) != 1) &&
            (get_at_col_row(position, base_cidx + 1, base_ridx - 1) != 1) &&
            (get_at_col_row(position, base_cidx + 2, base_ridx - 2) != 1) &&
            (get_at_col_row(position, base_cidx + 3, base_ridx - 3) != 1)) {
                contiguous_pieces = ((get_at_col_row(position, base_cidx, base_ridx) == 2) +
                                     (get_at_col_row(position, base_cidx + 1, base_ridx - 1) == 2) +
                                     (get_at_col_row(position, base_cidx + 2, base_ridx - 2) == 2) +
                                     (get_at_col_row(position, base_cidx + 3, base_ridx - 3) == 2));
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
		if (get_at_col_row(position, cidx, ridx) != 0) {
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
