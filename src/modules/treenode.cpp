#include "../../includes/mcts.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <math.h>
#include <memory>
#include <vector>


MctsNode::MctsNode(Coord move, int played_by, int wins) 
    : move(move)
    , wins(wins)
    , uct(INFINITY)
    , played_by(played_by)
    , fuly_expanded(false)
    , terminal(false)
    , visit_count(0)
    , parent(NULL)
{}
MctsNode::MctsNode(const MctsNode& other) :
  wins(other.wins),
  uct(other.uct),
  visit_count(other.visit_count),
  move(other.move),
  played_by(other.played_by),
  parent(other.parent),
  children() 
{     // Initialize empty children vector
  for (const std::unique_ptr<MctsNode>& child : other.children) {
    children.push_back(std::make_unique<MctsNode>(MctsNode(*child)));
  }
}

#define BACKTRACK_LIMIT 10
int backs = 0;

void MctsNode::select(std::vector<MctsNode*>& fill) {
    fill.push_back(this);
    if (!this->fuly_expanded || this->children.size() == 0) {
        return;
    }

    float idx = -1;
    float big_value = -1;

    for (int i = 0; i < this->children.size(); i++) {
        if (this->children[i]->terminal == true) {
            continue;
        }

        float val = this->children[i]->uct;
        if (val > big_value || idx == -1) {
            idx = i;
            big_value = val;
        }
    }

    if (idx == -1) {
        if (this->parent == NULL || backs == BACKTRACK_LIMIT) {
            backs = 0;
            return;
        }
        this->terminal = true;
        fill.pop_back();
        fill.pop_back();
        this->parent->select(fill);
        backs ++;
        return;
    }
    
    return this->children[idx]->select(fill);
}

bool has_child_move(const MctsNode * const node, Coord move) {
    for (auto&c: node->children) {
        if (c->move == move) {
            return true;
        }
    }
    return false;
}

void MctsNode::expand_simulate_backpropagte(Game& game, MctsCpuPlayer * self) {
    // std::cout << this->move.toString() << '\n';
    for (int i = 0; i < GAME_N; i++) {
        for (int j = 0; j < GAME_N; j++) {
            if (game.board[Coord{i, j}] != game.board.empty_square_marker
                    || game.isValid(Coord{i, j}, game.curr_idx) == false
                    || has_child_move(this, {i, j})
                ){
                continue;
            }
            int played_by = game.curr_idx;
            int p = game.play({i, j});
            int playout_wins = 0;

            for (int count = 0; count < SIM_COUNT; count++) {
                // aval is rollout function:
                //  1 - this play eventually wins
                //  0 - this play eventually draws            
                //  1 - this play eventually loses
                if (self->aval(game, self) == 1) {
                    playout_wins ++;
                } 
            }
            this->add_child({i, j}, played_by, playout_wins);
            this->backpropagate(&game, self, playout_wins);
            game.undo();
            
            if (p == -2) {
                this->children.back()->terminal = true;
            }

            return;
        }
    }
    this->fuly_expanded = true;
}

void MctsNode::backpropagate(const Game * const game, MctsCpuPlayer * self, int win_count) {
    this->visit_count += SIM_COUNT;
    this->wins += win_count;


    if (this->parent == NULL) {
        return;
    }
    // std::cout << "incread " << this->move.toString() << "'s wins count(" << this->wins - win_count << ") by " << win_count << " parent: " << this->parent->move.toString() << ", depth: " << depth << "\n";

    this->self_aval(game, self, win_count);
    this->parent->backpropagate(game, self, win_count);
}

void MctsNode::self_aval(const Game * const game, const MctsCpuPlayer * self, int win_count) {
    float win_ratio = (float)this->wins / this->visit_count + 1;
    
    float mod = self->aval_matrix[this->move.row][this->move.col];
    if (mod != 100 && mod >= 1) {
        mod = 1;
    }
    
    this->uct = (win_ratio + (EXPLOIT_EXPLORE) * sqrt(log(this->parent->visit_count + 1) / (this->visit_count + 1))) * mod;
}


void MctsNode::add_child(Coord move, int played_by, int win_count) {
    // std::cout << "added child: " << move.toString() << " to: " << this->move.toString() << "\n";

    auto c = std::make_unique<MctsNode>(MctsNode(move, played_by, win_count));
    c->parent = this;
    this->children.push_back(std::move(c));
}
