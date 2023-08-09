#include "chess.h"

int main() {
    ChessGame chess = ChessGame();
    chess.set_board();
    chess.print_board();

    //chess.move_piece(Team::WHITE,"a2","d2");// invalid move
    chess.move_piece(Team::WHITE,"b2","c3");// valid move
    chess.move_piece(Team::WHITE,"c3","d4");// valid move
    chess.move_piece(Team::WHITE,"d4","a7");// valid move
    chess.move_piece(Team::WHITE,"a7","f2");// valid move
    chess.move_piece(Team::WHITE,"f2","h4");// valid move
    chess.move_piece(Team::WHITE,"b1","d3");// invalid move

    //
    chess.move_piece(Team::BLACK,"f2","d3");// valid move
    chess.move_piece(Team::BLACK,"f2","h2");// invalid move
    return 0;
}
