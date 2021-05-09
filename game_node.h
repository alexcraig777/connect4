#ifndef GAME_NODE_H
#define GAME_NODE_H

#include "frame.h"

#include <stddef.h>

struct GameNode {
    struct FramePosition* position;
    struct GameNode* parent;
    int expected_payoff;
    
    struct GameNode* children[7];
    size_t most_promising_child;
};

struct GameNode* new_game_node(struct GameNode* parent,
			       struct FramePosition* position);

void free_game_node(struct GameNode* node);

void expand_node(struct GameNode* node);

void update_expected_payoff(struct GameNode* node);

struct GameNode* find_most_promising_leaf_descendant(struct GameNode* root);

#endif
