#include "chess.h"

int main() {
    pos_t to,from;
    bool turn = true;
    ChessGame chess = ChessGame();
    chess.set_board();
    chess.print_board();

    //chess.move_piece(Team::WHITE,"a2","d2");// invalid move
    /*
    chess.move_piece(Team::WHITE,"b2","c3");// valid move
    chess.move_piece(Team::WHITE,"c3","d4");// valid move
    chess.move_piece(Team::WHITE,"d4","a7");// valid move
    chess.move_piece(Team::WHITE,"a7","f2");// valid move
    chess.move_piece(Team::WHITE,"f2","h4");// valid move
    chess.move_piece(Team::WHITE,"c2","a3");// valid move
    chess.move_piece(Team::WHITE,"b1","d3");// invalid move
    */

    while (true) {
        cout "Move from: ";
        std::cin >> from;
        cout "Move to: ";
        std::cin >> to;

        if (turn) {
            while (0 > chess.move_piece(Team::WHITE,from,to)) {
                cout "Invalid move, try again..." endl;
                cout "Move from: ";
                std::cin >> from;
                cout "Move to: ";
                std::cin >> to;
            }
        }
        /*
        if (!turn) {
            while (0 > chess.move_piece(Team::BLACK,from,to")) {
                cout "Invalid move, try again..." endl;
            }
        }
        */
        //turn = !turn;
    }
    //
    chess.move_piece(Team::BLACK,"f2","d3");// valid move
    chess.move_piece(Team::BLACK,"f2","h2");// invalid move
    return 0;
}
