#include "../includes/game.h"
#include "../includes/cpuplayer.h"
#include <algorithm>
#include <chrono>
#include <any>
#include <array>
#include <memory>
#include <string>
#include <vector>
#include "../includes/inja.hpp"
#include "../includes/json.hpp"
#include "../includes/crow_all.h"
#include "../includes/bot_builders.cpp"
#include "../includes/avals.cpp"

#define TEMPLATES "./templates/"
#define LEADERBOARD_SIZE 10

struct leader_board_position {
    std::string holder;
    int time;
};

bool compare_leader_board_position(struct leader_board_position a, struct leader_board_position b) {
    return a.time < b.time;
}

int main() {
    std::vector<std::unique_ptr<Game>> games = {};
    std::vector<std::string> keys = {};
    std::vector<struct leader_board_position> leaderboard;

    auto updateLeaderBoard = [&leaderboard](std::string nome, int tentativa_tempo) {
        struct leader_board_position you = {nome, tentativa_tempo};
        std::vector<struct leader_board_position> cp = leaderboard;
        cp.push_back(you);
        std::sort(cp.begin(), cp.end(), compare_leader_board_position);

        if (cp.size() <= LEADERBOARD_SIZE) {
            leaderboard = cp;
            return;
        }

        for (int i = 0; i<LEADERBOARD_SIZE; i++) {
            leaderboard[i] = cp[i];
        }
    };

    auto printLeaderBoard = [&leaderboard]() {
        for (auto const p : leaderboard) {
            std::cout << "name: " << p.holder << " time: " << p.time << "\n";
        }
    };

    crow::App<crow::CookieParser> app;

    // CROW_ROUTE(app, "/")([](){
    //         inja::Environment env {TEMPLATES};

    //         inja::json data;
    //         auto page = env.render_file("intro.html", data);

    //         return page;
    //         });
    CROW_ROUTE(app, "/add-key/<string>")([&keys](std::string key){
            crow::response res;
            for (const std::string x: keys) {
                if (x == key) {
                    res.code = 401;
                    res.write("key already exists");
                    return res;
                }
            }
            res.code = 200;
            keys.push_back(key);
            return res;
            });
    CROW_ROUTE(app, "/game-creator")([&keys](const crow::request& req){
            inja::Environment env {TEMPLATES};

            auto qstr = req.url_params;
            auto key = std::string(qstr.get("key"));

            crow::response res;
            bool found = false; 
            for (const auto &x: keys) {
                if (x == key) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                res.code = 401;
                res.write("invalid key");
                return res;
            }

            inja::json data;
            auto page = env.render_file("index.html", data);
            res.write(page);

            return res;
            });
    CROW_ROUTE(app, "/new-game/<int>/<int>")([&games](int id, int arg){
            inja::Environment env {TEMPLATES};
            crow::response res;
            
            std::vector<std::any> args = {'x'};
            if (id == 4) {
                args.push_back(aval);
                args.push_back(arg);
            } else if (5) {
                args.push_back(rollout);
                args.push_back(arg);
                args.push_back(0); // suposed to be the idx of the mcts player, not actully used for anything
            }
            
            games.push_back(std::make_unique<Game>(new_game(0, id, {'o'}, args)));
            int index = games.size() - 1;
            std::string cookie = "game=" + std::to_string(index);
            res.set_header("Set-Cookie",  cookie+"; Path=/");

            inja::json data;

            std::array<std::array<char, 8>, 8> b = games[index]->board.board;
            for (auto c: games[index]->available()) {
                b[c.row][c.col] = 'a';
            }

            data["board"] = b;
            data["empty"] = games[index]->board.empty_square_marker;
            data["player1"] = games[index]->players[0]->piece;
            data["player2"] = games[index]->players[1]->piece;
            data["available"] = 'a';
            data["p1_count"] = games[index]->players[0]->piece_count;
            data["p2_count"] = games[index]->players[1]->piece_count;
            data["turn"] = games[index]->players[games[index]->curr_idx]->piece;
            auto page = env.render_file("game.html", data);
            res.write(page);

            return res; 
            });

    CROW_ROUTE(app, "/play/<int>/<int>")([&](const crow::request& req, int i, int j){
            inja::Environment env {TEMPLATES};
            inja::json data;
            crow::response res;

            auto& ctx = app.get_context<crow::CookieParser>(req);
            int index = std::stoi(ctx.get_cookie("game"));
            auto& g = games[index];

            if (i == -4 && j == -4) { // /play/-4/-4 means give up

                data["player1"] = g->players[0]->piece;
                data["player2"] = g->players[1]->piece;
                data["available"] = 'a';
                data["p1_count"] = g->players[0]->piece_count;
                data["p2_count"] = g->players[1]->piece_count;
                
                int winner_idx = g->players[1]->piece_count > g->players[0]->piece_count;
                int reverse = g->players[0]->piece_count > g->players[1]->piece_count;

                std::string end_text;

                if (winner_idx == reverse) {
                    end_text = "\n\n The Grame Drawed \n\n";
                } else {
                    std::ostringstream oss;
                    oss << "\n\n The Winner of game " << " is " << g->players[winner_idx]->piece << "\n\n";
                    end_text = oss.str();
                }

                data["end-text"] = end_text;
                data["winner"] = g->players[winner_idx]->piece;
                auto page = env.render_file("end-game.html", data);

                res.write(page);
                
                return res;
            }

            int over = g->play(Coord{i, j});

            // std::array<std::array<char, 8>, 8> b = games[index]->board.board;
            // for (auto c: games[index]->available()) {
            //     b[c.row][c.col] = 'a';
            // }
            
            data["board"] = g->board.board;
            data["empty"] = g->board.empty_square_marker;
            data["player1"] = g->players[0]->piece;
            data["player2"] = g->players[1]->piece;
            data["available"] = 'a';
            data["p1_count"] = g->players[0]->piece_count;
            data["p2_count"] = g->players[1]->piece_count;
            data["turn"] = g->players[g->curr_idx]->piece;
            auto page = env.render_file("game.html", data);

            res.add_header("HX-TRIGGER-AFTER-SETTLE", "update-cpu");
            res.write(page);
            return res;
            });

    CROW_ROUTE(app, "/bot")([&](const crow::request& req){
            inja::Environment env {TEMPLATES};
            inja::json data;

            auto& ctx = app.get_context<crow::CookieParser>(req);
            int index = std::stoi(ctx.get_cookie("game"));
            auto& g = games[index];

            if (g->isOver()) {
                int winner_idx = g->players[1]->piece_count > g->players[0]->piece_count;
                int reverse = g->players[0]->piece_count > g->players[1]->piece_count;

                if (g->players[winner_idx]->piece == 'o') { // winer is player not bot
                    std::cout << "we have a winner\n";
                    auto now = std::chrono::steady_clock::now();
                    std::chrono::duration<double> elapsed = now - g->moment_start;
                    int time_since_start = elapsed.count();
                    std::string nome = ctx.get_cookie("name");
                    updateLeaderBoard(nome, time_since_start);
                    printLeaderBoard();
                } 
                

                data["player1"] = g->players[0]->piece;
                data["player2"] = g->players[1]->piece;
                data["available"] = 'a';
                data["p1_count"] = g->players[0]->piece_count;
                data["p2_count"] = g->players[1]->piece_count;
                

                std::string end_text;

                if (winner_idx == reverse) {
                    end_text = "\n\n The Grame Drawed \n\n";
                } else {
                    std::ostringstream oss;
                    oss << "\n\n The Winner of game " << " is " << g->players[winner_idx]->piece << "\n\n";
                    end_text = oss.str();
                }

                data["end-text"] = end_text;
                data["winner"] = g->players[winner_idx]->piece;
                auto page = env.render_file("end-game.html", data);

                return page;
            }

            if (g->players[g->curr_idx]->piece != 'o') {
                Coord move = g->players[g->curr_idx]->choseSquare(*g);
                std::cout << "bot played move: " << move.toString() << "\n";
                g->play(move);
            }

            
            std::array<std::array<char, 8>, 8> b = g->board.board;
            for (auto c: g->available()) {
                b[c.row][c.col] = 'a';
            }

            data["board"] = b;
            data["empty"] = g->board.empty_square_marker;
            data["player1"] = g->players[0]->piece;
            data["player2"] = g->players[1]->piece;
            data["available"] = 'a';
            data["p1_count"] = g->players[0]->piece_count;
            data["p2_count"] = g->players[1]->piece_count;
            data["turn"] = g->players[g->curr_idx]->piece;
            auto page = env.render_file("game.html", data);

            return page;
            });
CROW_ROUTE(app, "/leaderboard")([&leaderboard](){
    inja::Environment env{TEMPLATES};
    inja::json data;
    std::vector<std::string> names;
    std::vector<int> times;
    for (auto const p:leaderboard){
        names.push_back(p.holder);
        times.push_back(p.time);
    }
    data["names"] = names;
    data["times"] = times;
    return env.render_file("leaderboard.html", data);
});


    app.port(5000).multithreaded().run();
}
