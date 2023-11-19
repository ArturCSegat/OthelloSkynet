#include "../includes/game.h"
#include "../includes/cpuplayer.h"
#include <crow/app.h>
#include <iostream>
#include <memory>
#include "crow.h"
#include "../includes/inja.hpp"
#include "../includes/json.hpp"


int main() {
    auto p1 = std::make_unique<Player>(Player('o'));
    auto p2 = std::make_unique<MaybeEvenBetterCpuPlayer>(MaybeEvenBetterCpuPlayer('x'));

    auto g = Game(std::move(p1), std::move(p2), ' ');
    
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([&g](){
            inja::Environment env {"templates/"};

            inja::json data;
            data["board"] = g.board.board;
            data["empty"] = g.board.empty_square_marker;
            data["player1"] = g.players[0]->piece;
            data["player2"] = g.players[1]->piece;
            auto page = env.render_file("index.html", data);

            return page;
            });

    CROW_ROUTE(app, "/play/<int>/<int>")([&g](int i, int j){
            g.play(Coord{i, j});
            g.play(g.players[g.curr_idx]->choseSquare(g));

            inja::Environment env {"templates/"};
            inja::json data;
            data["board"] = g.board.board;
            data["empty"] = g.board.empty_square_marker;
            data["player1"] = g.players[0]->piece;
            data["player2"] = g.players[1]->piece;
            auto page = env.render_file("board.html", data);

            return page;
            });

    app.port(5000).multithreaded().run();
}
