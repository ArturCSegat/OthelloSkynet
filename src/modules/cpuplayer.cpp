#include "../../includes/treenode.h"
#include "../../includes/cpuplayer.h"
#include "../../includes/game.h"
#include <array>
#include <cmath>
#include <math.h>
#include <map>
#include <memory>
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <queue>
#include <vector>

BadCpuPlayer::BadCpuPlayer(char p) : Player(p) {}

Coord BadCpuPlayer::choseSquare(Game& game) {
    std::map<int, Coord> moves;
    for (int i = 0; i < GAME_N; i++) {
        for (int j = 0; j < GAME_N; j++) {
            if (game.board[Coord{i, j}] != game.board.empty_square_marker) {
                continue;
            }
            int flipped = game.flipedFromMove(Coord{i, j}, game.curr_idx); 
            if (flipped == 0) {
                continue;
            }
            
            moves[flipped] = Coord{i, j};
        }
    }

    if (moves.empty()) {
        std::cout << "Skipping my turn\n";
        return Coord{-1, -1};
    }

    Coord best_move = moves[moves.rbegin()->first];

    std::cout << "Playing " << best_move.toString() << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return best_move;

}


CpuPlayer::CpuPlayer(char p) : Player(p) {
    int bg_idx = GAME_N - 1;

    for (int i = 0; i < GAME_N; i++) {
        for (int j = 0; j < GAME_N; j++) {
            aval_matrix[i][j] = 15;
        }
    }

    int mid_sm_idx = (GAME_N - 1) / 2;
    int mid_bg_idx = mid_sm_idx + 1;

    aval_matrix[mid_sm_idx][mid_sm_idx] = 15;
    aval_matrix[mid_bg_idx][mid_bg_idx] = 15;
    aval_matrix[mid_sm_idx][mid_bg_idx] = 15;
    aval_matrix[mid_bg_idx][mid_sm_idx] = 15;

    // generate rings
    float bg_val = 5;
    float sm_val = 0;
    int i = 0;
    for (;i < (GAME_N) / 2; i ++) {
        // good ring
        aval_matrix[i][0] = bg_val;
        aval_matrix[i][bg_idx] = bg_val;
        aval_matrix[0][i] = bg_val;
        aval_matrix[bg_idx][i] = bg_val;

        // bad ring
        aval_matrix[i][1] = sm_val;
        aval_matrix[i][bg_idx - 1] = sm_val;
        aval_matrix[1][i] = sm_val;
        aval_matrix[bg_idx - 1][i] = sm_val;

        sm_val += 0.25;
        bg_val += 5;
    }
    
    // mirrored opposite
    for (; i < GAME_N; i ++) {
        sm_val -= 0.25;
        bg_val -= 5;

        // good ring
        aval_matrix[i][0] = bg_val;
        aval_matrix[i][bg_idx] = bg_val;
        aval_matrix[0][i] = bg_val;
        aval_matrix[bg_idx][i] = bg_val;

        // bad ring
        aval_matrix[i][1] = sm_val;
        aval_matrix[i][bg_idx - 1] = sm_val;
        aval_matrix[1][i] = sm_val;
        aval_matrix[bg_idx - 1][i] = sm_val;
    }

    
    // modifiers
    aval_matrix[0][0] = 100.0;
    aval_matrix[0][bg_idx] = 100.0;
    aval_matrix[bg_idx][0] = 100.0;
    aval_matrix[bg_idx][bg_idx] = 100.0;
    
    aval_matrix[0][1] = 0.25;
    aval_matrix[1][0] = 0.25;
    aval_matrix[1][1] = 0.25;

    aval_matrix[0][bg_idx - 1] = 0.25;
    aval_matrix[1][bg_idx] = 0.25;
    aval_matrix[1][bg_idx - 1] = 0.25;

    aval_matrix[bg_idx - 1][0] = 0.25;
    aval_matrix[bg_idx][1] = 0.25;
    aval_matrix[bg_idx - 1][1] = 0.25;

    aval_matrix[bg_idx][bg_idx - 1] = 0.25;
    aval_matrix[bg_idx - 1][bg_idx - 1] = 0.25;
    aval_matrix[bg_idx - 1][bg_idx] = 0.25;

    // printing
    // for (int i = 0; i < GAME_N; i++) {
    //     for (int j = 0; j < GAME_N; j++) {
    //         // Add preceding zeros using setfill and setw
    //         std::cout << std::fixed << std::setprecision(2) << std::setfill('0') << std::setw(6) << std::right << aval_matrix[i][j] << " ";
    //     }
    //     std::cout << "\n";
    // }
    // exit(0);
}

Coord CpuPlayer::choseSquare(Game& game) {
    std::map<float, Coord> moves;
    for (int i = 0; i < GAME_N; i++) {
        for (int j = 0; j < GAME_N; j++) {
            if (game.board[Coord{i, j}] != game.board.empty_square_marker){
                continue;
            }
            int flipped = game.flipedFromMove(Coord{i, j}, game.curr_idx); 
            if (flipped == 0) {
                continue;
            }
            
            float positional_factor = 1.5 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2-1.5))); 
            // positional_factor = 2;
            float fit = std::pow(aval_matrix[i][j], positional_factor) + flipped;

            moves[fit] = Coord{i, j};
        }
    }

    if (moves.empty()) {
        return Coord{-1, -1};
    }

    Coord best_move = moves[moves.rbegin()->first];
    return best_move;
}



BetterCpuPlayer::BetterCpuPlayer(char p) : CpuPlayer(p) {};
Coord BetterCpuPlayer::choseSquare(Game& game) {
    std::map<float, Coord> moves;
    int c = 0;
    for (int i = 0; i < GAME_N; i++) {
        for (int j = 0; j < GAME_N; j++) {
            if (game.board[Coord{i, j}] != game.board.empty_square_marker
                    || game.flipedFromMove(Coord{i, j}, game.curr_idx) == 0){
                continue;
            }
            moves[avaliateMoveTillEnd(Coord{i, j}, std::move(game.clone()))] = Coord{i, j};
        }
    }

    if (moves.empty()) {
        // std::cout << "Skipping my turn\n";
        return Coord{-1, -1};
    }
    
    // for (const auto &[fit, move]: moves) {
    //     std::cout << "move: " << move.toString() << " wins: " << fit << "\n";
    // }

    Coord best_move = moves[moves.rbegin()->first];

    // std::cout << "Playing " << best_move.toString() << "\n";
    // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return best_move;
}

int BetterCpuPlayer::avaliateMoveTillEnd(Coord move, std::unique_ptr<Game> game) {
    // std::cout << "move: " << move.toString() << " avaliateMoveTillEnd\n";
    game->play(move);
    Coord sq;
    do{
        Coord tmp = CpuPlayer::choseSquare(*game);
        // not sure why but it eliminates bad sub-trees i guess;
        if (tmp == sq) {
            break;
        }
        sq = tmp;
    }while(game->play(sq) != -2);

    int this_idx = game->players[1]->piece == this->piece;

    return game->players[this_idx]->piece_count - game->players[!this_idx]->piece_count;
}



MaybeEvenBetterCpuPlayer::MaybeEvenBetterCpuPlayer(char p) : BetterCpuPlayer(p) {};

Coord MaybeEvenBetterCpuPlayer::choseSquare(Game& game) {
    std::map<int, Coord> moves;
    for (int i = 0; i < GAME_N; i++) {
        for (int j = 0; j < GAME_N; j++) {
            if (game.board[Coord{i, j}] != game.board.empty_square_marker
                    || game.flipedFromMove(Coord{i, j}, game.curr_idx) == 0){
                continue;
            }
            moves[avaliateShallowTreeTillEnd(Coord{i, j}, game.clone())] = Coord{i, j};
        }
    }

    if (moves.empty()) {
        std::cout << "Skipping my turn\n";
        return Coord{-1, -1};
    }
    
    // for (const auto &[fit, move]: moves) {
    //     std::cout << "move: " << move.toString() << " wins: " << fit << "\n";
    // }

    Coord best_move = moves[moves.rbegin()->first];

    // std::cout << "Playing " << best_move.toString() << "\n";
    // std::this_thread::sleep_for(std::chrono::milliseconds(250));
    return best_move;
}

int MaybeEvenBetterCpuPlayer::avaliateShallowTreeTillEnd(Coord move, std::unique_ptr<Game> game) {
    game->play(move);
    
    int aval_sum;
    
    for (int i = 0; i < GAME_N; i++) {
        for (int j = 0; j < GAME_N; j++) {
            if(game->board[Coord{i, j}] != game->board.empty_square_marker
                    || game->flipedFromMove(Coord{i, j}, game->curr_idx) == 0) {
                continue;
            }
            int fit = avaliateMoveTillEnd(Coord{i, j}, game->clone());
            aval_sum += fit;
        }
    }

    return aval_sum;
}

MinMaxCpuPlayer::MinMaxCpuPlayer(char p, float(*aval)(Game& game, const MinMaxCpuPlayer *const self), int max_depth) : BetterCpuPlayer(p) { 
    this->aval = aval;
    this->max_depth = max_depth;
};

Coord MinMaxCpuPlayer::choseSquare(Game& game) {
    int this_idx = game.players[1]->piece == this->piece;
    if (this_idx == 1) {
        float max_aval = MAXFLOAT * -1;
        auto max_move = Coord{-1, -1};

        for (int i = 0; i < GAME_N; i++) {
            for (int j = 0; j < GAME_N; j++) {
                if (game.board[Coord{i, j}] != game.board.empty_square_marker
                        || game.flipedFromMove(Coord{i, j}, game.curr_idx) == 0){
                    continue;
                }

                auto p1 = game.players[0]->piece_count;
                auto p2 = game.players[1]->piece_count;

                float fit = Min(game, Coord{i, j}, MAXFLOAT * -1, MAXFLOAT, 0);
                game.undo();

                // std::cout << "Move: " << Coord{i, j}.toString() << " aval: " << fit << "\n";

                auto p12 = game.players[0]->piece_count;
                auto p22 = game.players[1]->piece_count;

                if (p1 != p12 || p2 != p22) {
                    std::cout << "\nerror\n\n";
                    std::cout << "p1 :" << p1 << " p12: " << p12 << "\n";
                    std::cout << "p2 :" << p2 << " p22: " << p22 << "\n";
                    std::cout << "\nerror\n\n";
                    std::cout << "last failed coordinate: " << Coord{i, j}.toString() << "\n";
                    exit(1);
                }

                if (fit > max_aval) {
                    max_aval = fit;
                    max_move = Coord{i, j};
                }
            }
        }
        return max_move;
    }
    float max_aval = std::numeric_limits<float>::max();
    auto max_move = Coord{-1, -1};

    for (int i = 0; i < GAME_N; i++) {
        for (int j = 0; j < GAME_N; j++) {
            if (game.board[Coord{i, j}] != game.board.empty_square_marker
                    || game.flipedFromMove(Coord{i, j}, game.curr_idx) == 0){
                continue;
            }

            auto p1 = game.players[0]->piece_count;
            auto p2 = game.players[1]->piece_count;

            float fit = Max(game, Coord{i, j}, MAXFLOAT * -1, MAXFLOAT, 0);
            game.undo();

            // std::cout << "Move: " << Coord{i, j}.toString() << " aval: " << fit << "\n";

            auto p12 = game.players[0]->piece_count;
            auto p22 = game.players[1]->piece_count;

            if (p1 != p12 || p2 != p22) {
                std::cout << "\nerror\n\n";
                std::cout << "p1 :" << p1 << " p12: " << p12 << "\n";
                std::cout << "p2 :" << p2 << " p22: " << p22 << "\n";
                std::cout << "\nerror\n\n";
                std::cout << "last failed coordinate: " << Coord{i, j}.toString() << "\n";
                exit(1);
            }

            if (fit < max_aval) {
                max_aval = fit;
                max_move = Coord{i, j};
            }
        }
    }
    return max_move;
}

float MinMaxCpuPlayer::Max(Game& game, Coord move, float alpha, float beta, int depth) {
    // std::cout << "Max: move: " << move.toString() << ", depth: " << depth << "\n";

    // std::cout << "MAx turn: " << game.curr_idx <<  "depth: " << depth <<"\n";
    float max_aval = MAXFLOAT * -1;
    float aval;
    int r = game.play(move);

    if (r == -2 || depth == this->max_depth) {
        return this->aval(game, this);
    }

    for (int i = 0; i < GAME_N; i++) {
        for (int j = 0; j < GAME_N; j++) {
            if (game.board[Coord{i, j}] != game.board.empty_square_marker
                    || game.flipedFromMove(Coord{i, j}, game.curr_idx) == 0){
                continue;
            }
            aval = Min(game, Coord{i, j}, alpha, beta, depth + 1);
            game.undo();
            alpha = std::max(alpha, aval);
            if (aval > max_aval) {
                max_aval = aval;
            }
            
            if (beta <= alpha) {
                return max_aval;
            }
        }
    }
    return max_aval;
}


float MinMaxCpuPlayer::Min(Game& game, Coord move, float alpha, float beta, int depth) {
    // std::cout << "Min: move: " << move.toString() << ", depth: " << depth << "\n";

    // std::cout << "Min turn: " << game.curr_idx <<  "depth: " << depth <<"\n";
    float min_aval = MAXFLOAT;
    float aval;
    int r = game.play(move);

    if (r == -2 || depth == this->max_depth) {
        return this->aval(game, this);
    }

    for (int i = 0; i < GAME_N; i++) {
        for (int j = 0; j < GAME_N; j++) {
            if (game.board[Coord{i, j}] != game.board.empty_square_marker
                    || game.flipedFromMove(Coord{i, j}, game.curr_idx) == 0){
                continue;
            }
            aval = Max(game, Coord{i, j}, alpha, beta, depth + 1);
            game.undo();
            if (aval < min_aval) {
                min_aval = aval;
            }
            
            beta = std::min(beta, aval);
            if (beta <= alpha) {
                return min_aval;
            }
        }
    }
    return min_aval;
}

MctsCpuPlayer::MctsCpuPlayer(
        char p,
        float(*aval)(Game& game, const MinMaxCpuPlayer *const self),
        int max_depth,
        int idx
        )
    : MinMaxCpuPlayer(p, aval, max_depth)
    , game_idx(idx)
{}

Coord MctsCpuPlayer::choseSquare(Game& game) {
        auto root = MctsNode({-10, -10}, game.curr_idx, MAXFLOAT * -1); // bad move so it errors and keeps the turn on the same player
        std::vector<MctsNode*> curr = {};

        int iter_count = 0;

        while(iter_count < this->max_depth) {
            // std::cout << "pre select\n";
            // for (auto const& c: root.children) {
            //     std::cout << c->move.toString() << "> wins: " << c->wins << ", uct: " << c->uct << ", visits: " << c->visit_count << "\n"; 
            // }
            root.select(curr);
            for (auto& n: curr) {
                game.play(n->move);
            }

            curr.back()->expand_simulate_backpropagte(game, this);

            // start at first index to skip undoing root
            for (int i = 1; i < curr.size(); i++) {
                game.undo();
            }

            curr.clear();
            iter_count ++;
        }

        if (root.children.empty()) {
            return {-1, -1};
        }
        for (auto const& c: root.children) {
            std::cout << c->move.toString() << "> wins: " << c->wins << ", uct: " << c->uct << ", visits: " << c->visit_count << "\n"; 
            for (auto const& cc: c->children) {
                std::cout << cc->move.toString() << "> wins: " << cc->wins << ", uct: " << cc->uct << ", visits: " << cc->visit_count << "\n"; 
            }
        }
        root.select(curr);

        return curr[1]->move;
    }

    MctsCpuPlayer3::MctsCpuPlayer3(
            char p,
            float(*aval)(Game& game, const MinMaxCpuPlayer *const self),
            int max_depth,
            int idx
            )
            : MctsCpuPlayer(p, aval, max_depth, idx) 
    {
        this->tree_root = std::make_unique<MctsNode>(MctsNode({-10, -10}, idx, 0));
    }

    MctsCpuPlayer3::MctsCpuPlayer3(const MctsCpuPlayer3& other) :
      MctsCpuPlayer(other), // Call base class copy constructor
      tree_root(std::make_unique<MctsNode>(*other.tree_root)) { // Deep copy of tree_root
    }

Coord MctsCpuPlayer3::choseSquare(Game& game) {
    Coord last_move = {-10, -10};

    if (!game.flips.empty()) {
        last_move = game.flips.top().back();
    }


    this->step(last_move);
    // std::cout << "new root is " << this->tree_root->move.toString() << "\n new root has children\n";
    // for (auto&c : this->tree_root->children) {
    //     std::cout << c->move.toString() << " ";
    // }
    // std::cout << "\n";

    std::vector<MctsNode*> curr;
    curr.reserve(10);

    int iter_count = 0;

    int safe = game.players[0]->piece_count;
    while(iter_count < this->max_depth && !this->tree_root->terminal) {
        this->tree_root->select(curr);
        if (game.players[0]->piece_count != safe) {
            std::cout << "broke at 1\n";
            exit(1);
        }

        // for (auto c: curr) {
        //     std::cout << c->move.toString() << "= " << c->wins << " ,";
        // }
        // std::cout << "\n";

        for (auto& n: curr) {
            game.play(n->move);
        }

        curr.back()->expand_simulate_backpropagte(game, this);

        // for (auto c: curr) {
        //     std::cout << c->move.toString() << "= " << c->wins << " ,";
        // }
        // std::cout << "\n";

        // start at first index to skip undoing root
        for (int i = 1; i < curr.size(); i++) {
            game.undo();
        }
        if (game.players[0]->piece_count != safe) {
            std::cout << "broke at 2\n";

            for (auto c: curr) {
                std::cout << c->move.toString() << "= " << c->wins << " ,";
            }
            std::cout << "\n";


            exit(1);
        }

        curr.clear();
        iter_count ++;
    }

    if (this->tree_root->children.empty()) {
        return {-1, -1};
    }
    std::cout << "root: " << this->tree_root->move.toString() << "> wins: " << this->tree_root->wins << ", uct: " << this->tree_root->uct << ", visits: " << this->tree_root->visit_count << "\n"; 
    for (auto const& c: this->tree_root->children) {
        std::cout << c->move.toString() << "> wins: " << c->wins << ", uct: " << c->uct << ", visits: " << c->visit_count << " children: " << c->children.size() << "\n"; 
    }
    std::cout << "root: " << this->tree_root->move.toString() << "> wins: " << this->tree_root->wins << ", uct: " << this->tree_root->uct << ", visits: " << this->tree_root->visit_count << "children: " << this->tree_root->children.size() << "\n"; 

    this->tree_root->select(curr);
    Coord move;
    if (curr.size() > 1) {
        move = curr[1]->move;
    } else {
        float idx = -1;
        float big_value = -1;

        for (int i = 0; i < this->tree_root->children.size(); i++) {
            float val = this->tree_root->children[i]->uct;
            if (val > big_value || idx == -1) {
                idx = i;
                big_value = val;
            }
        }
        move = this->tree_root->children[idx]->move;
    }

    this->step(move);

    return move;
}

void MctsCpuPlayer3::step(Coord move) {
    for (int i = 0; i < this->tree_root->children.size(); i++) {
        if (this->tree_root->children[i]->move == move) {
            this->tree_root = std::move(this->tree_root->children[i]);
            this->tree_root->parent = NULL;

            // std::cout << "new root->\n";
            // std::cout << "root: " << this->tree_root->move.toString() << "> children: " << this->tree_root->children.size() << ", uct: " << this->tree_root->uct << ", visits: " << this->tree_root->visit_count << "\n"; 
            // for (auto const& c: this->tree_root->children) {
            //     std::cout << c->move.toString() << "> children: " << c->children.size() << ", uct: " << c->uct << ", visits: " << c->visit_count << " children: " << c->children.size() << "\n"; 
            // }
            // std::cout << "root: " << this->tree_root->move.toString() << "> wins: " << this->tree_root->wins << ", uct: " << this->tree_root->uct << ", visits: " << this->tree_root->visit_count << "children: " << this->tree_root->children.size() << "\n"; 

            return;
        }
    }
    this->tree_root = std::make_unique<MctsNode>(MctsNode({-10, -10}, this->tree_root->played_by, 0));
}
