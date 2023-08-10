#include "chess.h"

int main() {
    pos_t to,from;
    bool turn = true;
    ChessGame chess = ChessGame();
    chess.set_board();
    chess.print_board();

    while (true) {
        if (turn) {
            cout "Player One move." endl;
            cout "Move from: ";
            std::cin >> from;
            cout "Move to: ";
            std::cin >> to;
            while (0 > chess.move_piece(Team::WHITE,from,to)) {
                cout "Invalid move, try again..." endl;
                cout "Move from: ";
                std::cin >> from;
                cout "Move to: ";
                std::cin >> to;
            }
        }
        if (!turn) {
            cout "Player Two move." endl;
            cout "Move from: ";
            std::cin >> from;
            cout "Move to: ";
            std::cin >> to;
            while (0 > chess.move_piece(Team::BLACK,from,to)) {
                cout "Invalid move, try again..." endl;
                cout "Move from: ";
                std::cin >> from;
                cout "Move to: ";
                std::cin >> to;
            }
        }
        turn = !turn;
    }

    return 0;
}
