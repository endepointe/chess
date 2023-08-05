#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <map>

#define KING_WHITE      "\u2654" 
#define ROOK_WHITE      "\u2656"
#define KNIGHT_WHITE    "\u2658"
#define BISHOP_WHITE    "\u2657"

#define KING_BLACK      "\u265A" 
#define ROOK_BLACK      "\u265C"
#define KNIGHT_BLACK    "\u265E"
#define BISHOP_BLACK    "\u265D"
#define BOX             "\x1b[48;5;232m"
#define COLOR_RESET     "\x1b[0m"

using PIECE_T = std::string; 
using POS_T = std::string;

enum State {UNFINISHED, WHITE_WON, BLACK_WON};
enum Team {WHITE, BLACK};
enum Piece {KING,KNIGHT,ROOK,BISHOP};

class ChessTeam {
    private:
    std::map<Team, std::string> team_map {
        {Team::WHITE, "WHITE"},
        {Team::BLACK, "BLACK"}
    };
    std::map<Piece, std::string> piece_map;
    std::string team;
    std::vector<Piece> pieces = {KING,KNIGHT,KNIGHT,BISHOP,BISHOP,ROOK};

    public:
    ChessTeam(Team t) : team(team_map[t]) {
        if (team == "WHITE") {
            piece_map = {
                {Piece::KING, KING_WHITE},
                {Piece::KNIGHT, KNIGHT_WHITE},
                {Piece::ROOK, ROOK_WHITE},
                {Piece::BISHOP, BISHOP_WHITE}
            };
        } else {
            piece_map = {
                {Piece::KING, KING_BLACK},
                {Piece::KNIGHT, KNIGHT_BLACK},
                {Piece::ROOK, ROOK_BLACK},
                {Piece::BISHOP, BISHOP_BLACK}
            };
        }
    } 
    ~ChessTeam() {}
    std::string get_team() {
        return team;
    }
    void get_pieces() {
        for (Piece p : pieces) {
            std::cout << piece_map[p] << " ";
        }
        std::cout << "\n";
    }
    POS_T get_piece(Piece p) {
        return piece_map[p];
    }
};

class ChessGame {
    private:
    std::map<State, std::string> state_map = {
        {State::UNFINISHED, "UNFINISHED"},
        {State::WHITE_WON, "WHITE_WON"},
        {State::BLACK_WON, "BLACK_WON"}
    };
    std::string state;
    std::vector<std::tuple<POS_T, PIECE_T>> board;
    std::map<POS_T, unsigned int> starting_location = {
        {" ",0},
        {"a2",1},{"b2",2},{"c2",3},{"f2",4},{"g2",5},{"h2",6},
        {"a1",7},{"b1",8},{"c1",9},{"f1",10},{"g1",11},{"h1",12},
    };
    char alpha8[8] = {'a','b','c','d','e','f','g','h'};
    unsigned int num8[8] = {1,2,3,4,5,6,7,8};
    unsigned int size = 8;
    ChessTeam player_one = ChessTeam(Team::WHITE);
    ChessTeam player_two = ChessTeam(Team::BLACK);

    public:
    ChessGame();
    ~ChessGame();
    std::string get_game_state() {return state;}
    void set_game_state(State s) {
        state = state_map[s];
    }
    void set_board() {
        for (int row = size - 1; row >= 0; row--) {
            for (int col = 0; col < size; col++) {
                std::string loc = alpha8[col] + std::to_string(num8[row]);
                switch (starting_location[loc]) {
                    case 0:
                        board.push_back(std::make_tuple(loc, "*"));
                        break;
                    case 1:
                        board.push_back(std::make_tuple(loc, 
                                    player_one.get_piece(ROOK)));
                        break;
                    case 2:
                        board.push_back(std::make_tuple(loc, 
                                    player_one.get_piece(BISHOP)));
                        break;
                    case 3:
                        board.push_back(std::make_tuple(loc, 
                                    player_one.get_piece(KNIGHT)));
                        break;
                    case 4:
                        board.push_back(std::make_tuple(loc, 
                                    player_two.get_piece(KNIGHT)));
                        break;
                    case 5:
                        board.push_back(std::make_tuple(loc, 
                                    player_two.get_piece(BISHOP)));
                        break;
                    case 6:
                        board.push_back(std::make_tuple(loc, 
                                    player_two.get_piece(ROOK)));
                        break;
                    case 7:
                        board.push_back(std::make_tuple(loc, 
                                    player_one.get_piece(KING)));
                        break;
                    case 8:
                        board.push_back(std::make_tuple(loc, 
                                    player_one.get_piece(BISHOP)));
                        break;
                    case 9:
                        board.push_back(std::make_tuple(loc, 
                                    player_one.get_piece(KNIGHT)));
                        break;
                    case 10:
                        board.push_back(std::make_tuple(loc, 
                                    player_two.get_piece(KNIGHT)));
                        break;
                    case 11:
                        board.push_back(std::make_tuple(loc, 
                                    player_two.get_piece(BISHOP)));
                        break;
                    case 12:
                        board.push_back(std::make_tuple(loc, 
                                    player_two.get_piece(KING)));
                        break;
                    default:
                        //board.push_back(std::make_tuple(loc, " "));
                        break;
                }
            }
        }
    }
    
    std::vector<std::tuple<POS_T, PIECE_T>> print_board() {
        unsigned int max = 0;
        bool flip = false;
        for (std::tuple<POS_T, PIECE_T> n : board) {
            if (!flip) {
                std::cout << BOX << std::get<1>(n) << " " << COLOR_RESET;
            } else {
                std::cout << std::get<1>(n) << " ";
            }
            flip = !flip;
            if (max == 7) {
                std::cout << " \n";
                flip = !flip;
                max = 0;
            } else {
                max++;
            }
        }
        return board;
    }
};

ChessGame::ChessGame() {
    state = state_map[State::UNFINISHED];
}
ChessGame::~ChessGame() {
    // delete any memory allocations
}

int main() {
    ChessGame chess = ChessGame();
    chess.set_board();
    std::cout << chess.get_game_state() << std::endl;
    chess.print_board();
    chess.set_game_state(State::BLACK_WON);
    std::cout << chess.get_game_state() << std::endl;

    ChessTeam p1 = ChessTeam(Team::WHITE);
    ChessTeam p2 = ChessTeam(Team::BLACK);
    std::cout << p1.get_team() << "\n";
    std::cout << p2.get_team() << "\n";
    p1.get_pieces();
    p2.get_pieces();
    std::cout << p2.get_piece(KING) << "\n";
    std::cout <<"\x1b[48;5;232m" << "Opaque Grey Text" << "\x1b[0m" << std::endl;
    std::cout << (2 % 2) << std::endl;
    return 0;
}
