// #include <map>
// #include "../../includes/cpuplayer.h"
// #include "../../includes/game.h"
//
// MinMaxCpuPlayer::MinMaxCpuPlayer(char p) : CpuPlayer(p) {};
//
// Coord MinMaxCpuPlayer::choseSquare(const Game& game) {
//     std::map<float, Coord> moves;
//     for (int i = 0; i < 8; i++) {
//         for (int j = 0; j < 8; j++) {
//             if (game.board[Coord{i, j}] != game.board.empty_square_marker
//                     || game.flipedFromMove(Coord{i, j}).size() == 0){
//                 continue;
//             }
//             
//             auto clone = game.clone();
//             clone->play(Coord{i, j});
//             moves[Max(*clone, 0)] = Coord{i, j};
//         }
//     }
//
//     if (moves.empty()) {
//         return Coord{-1, -1};
//     }
//     
//     return moves[moves.rbegin()->first];
// }
//
// float MinMaxCpuPlayer::Max(Game& game, int depth) {
//     int max_aval = -999;
//
//     for (int i = 0; i < 8; i++) {
//         for (int j = 0; j < 8; j++) {
//             if (game.board[Coord{i, j}] != game.board.empty_square_marker
//                     || game.flipedFromMove(Coord{i, j}).size() == 0){
//                 continue;
//             }
//
//             auto clone = game.clone();
//             int r = clone->play(Coord{i, j});
//
//             int aval;
//             if (r == -2 || depth == MAX_DEPTH) {
//                 aval = clone->playerAval(this->piece);
//             } else {
//                 auto next_clone = clone->clone();
//                 aval = Min(*next_clone, depth + 1);
//             }
//             
//             if (aval > max_aval) {
//                 max_aval = aval;
//             }
//         }
//     }
//     return max_aval;
// }
//
//
// float MinMaxCpuPlayer::Min(Game& game, int depth) {
//     int max_aval = -999;
//
//     for (int i = 0; i < 8; i++) {
//         for (int j = 0; j < 8; j++) {
//             if (game.board[Coord{i, j}] != game.board.empty_square_marker
//                     || game.flipedFromMove(Coord{i, j}).size() == 0){
//                 continue;
//             }
//
//             auto clone = game.clone();
//             int r = clone->play(Coord{i, j});
//
//             int aval;
//             if (r == -2 || depth == MAX_DEPTH) {
//                 aval = clone->playerAval(this->piece);
//             } else {
//                 auto next_clone = clone->clone();
//                 aval = Max(*next_clone, depth + 1);
//             }
//             
//             if (aval > max_aval) {
//                 max_aval = aval;
//             }
//         }
//     }
//     return max_aval;
// }
