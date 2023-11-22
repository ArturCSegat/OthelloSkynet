#include "../includes/game.h"
#include "../includes/cpuplayer.h"
#include <iostream>
#include <memory>
#include "../includes/inja.hpp"
#include "../includes/json.hpp"
#include "../includes/crow_all.h"



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
            data["p1_count"] = g.players[0]->piece_count;
            data["p2_count"] = g.players[1]->piece_count;
            data["turn"] = g.players[g.curr_idx]->piece;
            auto page = env.render_file("index.html", data);

            return page;
            });

    CROW_ROUTE(app, "/play/<int>/<int>")([&g](int i, int j){
            int over = g.play(Coord{i, j});
            
            inja::Environment env {"templates/"};
            inja::json data;


            data["board"] = g.board.board;
            data["empty"] = g.board.empty_square_marker;
            data["player1"] = g.players[0]->piece;
            data["player2"] = g.players[1]->piece;
            data["p1_count"] = g.players[0]->piece_count;
            data["p2_count"] = g.players[1]->piece_count;
            data["turn"] = g.players[g.curr_idx]->piece;
            auto page = env.render_file("game-info.html", data);

            // playing after rendering the page will ensure the player
            // does not receive the up to date state of the board, but the changes
            // their play made, you should then call "/game" to get the updated with CPU move
            g.play(g.players[g.curr_idx]->choseSquare(g));
            return page;
            });

    CROW_ROUTE(app, "/game")([&g](){
            inja::Environment env {"templates/"};
            inja::json data;

            if (g.running == false) {
                data["player1"] = g.players[0]->piece;
                data["player2"] = g.players[1]->piece;
                data["p1_count"] = g.players[0]->piece_count;
                data["p2_count"] = g.players[1]->piece_count;
                
                int winner_idx = g.players[1]->piece_count > g.players[0]->piece_count;
                int reverse = g.players[0]->piece_count > g.players[1]->piece_count;

                std::string end_text;

                if (winner_idx == reverse) {
                    end_text = "\n\n The Grame Drawed \n\n";
                } else {
                    std::ostringstream oss;
                    oss << "\n\n The Winner of game " << " is " << g.players[winner_idx]->piece << "\n\n";
                    end_text = oss.str();
                }

                data["end-text"] = end_text;
                data["winner"] = g.players[winner_idx]->piece;
                auto page = env.render_file("end-game.html", data);
                return page;
            }


            data["board"] = g.board.board;
            data["empty"] = g.board.empty_square_marker;
            data["player1"] = g.players[0]->piece;
            data["player2"] = g.players[1]->piece;
            data["p1_count"] = g.players[0]->piece_count;
            data["p2_count"] = g.players[1]->piece_count;
            data["turn"] = g.players[g.curr_idx]->piece;
            auto page = env.render_file("game-info.html", data);

            return page;
            });


    app.port(5000).multithreaded().run();
}
