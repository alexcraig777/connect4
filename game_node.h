#ifndef GAME_NODE_H
#define GAME_NODE_H

#include "frame.h"

#include <stddef.h>

struct GameNode {
    struct FramePosition* position;
    struct GameNode* parent;

    int expected_payoff;
    int search_payoff;
    int search_penalty;
    
    struct GameNode* children[7];

    size_t preferred_move_child;
    size_t preferred_search_child;
};

struct GameNode* new_game_node(struct GameNode* parent,
			       struct FramePosition* position);

void free_game_node(struct GameNode* node);

char expand_node(struct GameNode* node);

void update_expected_payoff(struct GameNode* node);

struct GameNode* find_preferred_search_leaf(struct GameNode* root);

int get_best_move(struct GameNode* node);

#endif
