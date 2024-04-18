#include <any>
#include <memory>
#include <vector>
#include "game.h"
#include "player.h"
#include "cpuplayer.h"
#include "mcts.h"

using std::unique_ptr;
using std::make_unique;

unique_ptr<Player> new_player(char piece) {
    return make_unique<Player>(Player(piece));
}

unique_ptr<CpuPlayer> new_cpu_player(char piece) {
    return make_unique<CpuPlayer>(CpuPlayer(piece));
}

unique_ptr<BetterCpuPlayer> new_better_cpu_player(char piece) {
    return make_unique<BetterCpuPlayer>(BetterCpuPlayer(piece));
}

unique_ptr<MaybeEvenBetterCpuPlayer> new_maybe_even_better_cpu_player(char piece) {
    return make_unique<MaybeEvenBetterCpuPlayer>(MaybeEvenBetterCpuPlayer(piece));
}

unique_ptr<MinMaxCpuPlayer> new_min_max_cpu_player(char piece, float(*aval)(Game& game, const MinMaxCpuPlayer *const self), int max_depth) {
    return make_unique<MinMaxCpuPlayer>(MinMaxCpuPlayer(piece, aval, max_depth));
}

unique_ptr<MctsCpuPlayer> new_mcts_cpu_player(char piece, float(*aval)(Game& game, const MinMaxCpuPlayer *const self), int max_depth, int idx) {
    return make_unique<MctsCpuPlayer>(MctsCpuPlayer(piece, aval, max_depth, idx));
}

// too bad
Game new_game(int bot1_id, int bot2_id, std::vector<std::any> args1, std::vector<std::any> args2) {
    unique_ptr<Player> p1;
    unique_ptr<Player> p2;

    switch (bot1_id) {
        case 0:
            p1 = new_player(std::any_cast<char>(args1[0]));
            break;
        case 1:
            p1 = new_cpu_player(std::any_cast<char>(args1[0]));
            break;
        case 2:
            p1 = new_better_cpu_player(std::any_cast<char>(args1[0]));
            break;
        case 3:
            p1 = new_maybe_even_better_cpu_player(std::any_cast<char>(args1[0]));
            break;
        case 4:
            p1 = new_min_max_cpu_player(std::any_cast<char>(args1[0]), std::any_cast<float(*)(Game&, const MinMaxCpuPlayer *const)>(args1[1]), std::any_cast<int>(args1[2]));
            break;
        case 5:
            p1 = new_mcts_cpu_player(std::any_cast<char>(args1[0]), std::any_cast<float(*)(Game&, const MinMaxCpuPlayer *const)>(args1[1]), std::any_cast<int>(args1[2]), std::any_cast<int>(args1[3]));
            break;
    }

    switch (bot2_id) {
        case 0:
            p2 = new_player(std::any_cast<char>(args2[0]));
            break;
        case 1:
            p2 = new_cpu_player(std::any_cast<char>(args2[0]));
            break;
        case 2:
            p2 = new_better_cpu_player(std::any_cast<char>(args2[0]));
            break;
        case 3:
            p2 = new_maybe_even_better_cpu_player(std::any_cast<char>(args2[0]));
            break;
        case 4:
            p2 = new_min_max_cpu_player(std::any_cast<char>(args2[0]), std::any_cast<float(*)(Game&, const MinMaxCpuPlayer *const)>(args2[1]), std::any_cast<int>(args2[2]));
            break;
        case 5:
            p2 = new_mcts_cpu_player(std::any_cast<char>(args2[0]), std::any_cast<float(*)(Game&, const MinMaxCpuPlayer *const)>(args2[1]), std::any_cast<int>(args2[2]), std::any_cast<int>(args2[3]));
            break;
    }

    // Create the game with the chosen players
    return Game(std::move(p1), std::move(p2), ' ');
}
