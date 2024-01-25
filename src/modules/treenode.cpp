#include "../../includes/treenode.h"
#include <algorithm>
#include <math.h>
#include <memory>
#include <numeric>
#include <vector>

MctsNode::MctsNode(Coord move, int played_by, float playout_val) 
    : move(move)
    , value(playout_val)
    , uct(0)
    , played_by(played_by)
    , visit_count(0)
    , parent(NULL)
{}

void MctsNode::select(std::vector<MctsNode*>& fill) {
    fill.push_back(this);
    this->visit_count ++;
    if (this->children.empty()) {
        return;
    }

    float idx = 0;
    float big_value = this->children[0]->uct + this->children[0]->value;

    for (int i = 1; i < this->children.size(); i++) {
        if (this->children[i]->uct + this->children[i]->value > big_value) {
            idx = i;
            big_value = this->children[i]->uct + this->children[i]->value;
        }
    }

    this->children[idx]->select(fill);
}

void MctsNode::expand_simulate(Game& game, MctsCpuPlayer * self) {
    for (int i = 0; i < GAME_N; i++) {
        for (int j = 0; j < GAME_N; j++) {
            if (game.board[Coord{i, j}] != game.board.empty_square_marker
                    || game.flipedFromMove(Coord{i, j}, game.curr_idx).empty()){
                continue;
            }
            
            int pb = game.curr_idx;
            game.play({i, j});
            float value = 0;
            if (game.players[0]->piece_count + game.players[1]->piece_count >= 32) { // past mid game
                value = self->aval(game, self);
                if (value > 0) {
                    value = 1;
                } else if (value < 0) {
                    value = -1;
                }

            } else {
                value = self->rollout(game, self);
            }

            this->value = 1 * (this->value - value) / this->visit_count;
            this->children.push_back(std::make_unique<MctsNode>(MctsNode({i, j}, pb, value)));
            game.undo();
        }
    }
}

void MctsNode::backpropagate() {
    if (this->parent == NULL) {
        return;
    }

    this->self_aval();
    this->parent->backpropagate();
}

void MctsNode::self_aval() {
    MctsNode * root = this;
    while (root->parent != NULL) {
        root = root->parent;
    }

    auto SQRT2 = 1.41421356237;
    const auto C = 2;
    float children_win = 0;
    for (auto& c: this->children) {
        if (c->value > 0) {
            if (this->played_by == root->played_by){
                children_win++;
            }
        }
    }
    
    if (this->children.size() > 0) {
        children_win /= this->children.size();
    }
    
    
    this->value = children_win;
    this->uct = children_win + (0.25) * sqrt((log2(this->parent->visit_count) / this->visit_count));
}

