#include <stdexcept>
#include <iostream>
#include <string>
#include <cstdlib>
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

using piece_t = std::string; 
using pos_t = std::string;
using team_t = std::string;
using uint_t = unsigned int;

enum State {UNFINISHED, WHITE_WON, BLACK_WON};
enum Team {WHITE, BLACK};
enum Piece {KING,KNIGHT,BISHOP,ROOK};
struct PieceInfo {
    Piece name;
    pos_t pos;
    pos_t possible_move[4];
};

class ChessTeam {
    private:
    std::map<Team, team_t> team_map {
        {Team::WHITE, "WHITE"},
        {Team::BLACK, "BLACK"}
    };
    std::map<Piece, team_t> piece_map;
    std::string team;
    std::vector<Piece> piece_names = {KING,KNIGHT,KNIGHT,BISHOP,BISHOP,ROOK};
    std::vector<PieceInfo> pieces = {
        PieceInfo{KING,"",{"","","",""}},PieceInfo{KNIGHT,"",{"","","",""}},
        PieceInfo{KNIGHT,"",{"","","",""}},PieceInfo{BISHOP,"",{"","","",""}},
        PieceInfo{BISHOP,"",{"","","",""}},PieceInfo{ROOK,"",{"","","",""}},
    };

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

    std::vector<PieceInfo>& get_pieces() {
        return pieces;
    }

    piece_t get_piece_symbol(Piece p) {
        return piece_map[p];
    }

    void set_piece_position(PieceInfo& p, pos_t pos) {
        p.pos = pos;
    }

    pos_t get_piece_position(PieceInfo p) {
        return p.pos;
    }
    // this should be a fn for ChessGame
    piece_t get_piece_at_position(pos_t pos){
        for (PieceInfo p : pieces){
            if (p.pos == pos) {
                return piece_map[p.name];
            }
        }
        return "*";
    }
    void take_piece(ChessTeam &other_team, Piece p) {
        std::cout << "Team: " << get_team();
        std::cout << " takes:" << other_team.get_team();
        std::cout << "'s " << other_team.get_piece_symbol(p) << "\n";
        other_team.get_pieces();
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
    std::vector<std::tuple<pos_t, piece_t, team_t>> board;
    std::map<pos_t, unsigned int> starting_location = {
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
    ChessGame() {
        state = state_map[State::UNFINISHED];
    };
    ~ChessGame() {};
    std::string get_game_state() {return state;}
    void set_game_state(State s) {
        state = state_map[s];
    }
    ChessTeam get_player(Team t) {
        if (t == 0) {
            return player_one;
        }
        return player_two;
    }

    void set_board() {
        for (int row = size - 1; row >= 0; row--) {
            for (int col = 0; col < size; col++) {
                std::string loc = alpha8[col] + std::to_string(num8[row]);
                switch (starting_location[loc]) {
                    case 0:
                        board.push_back(std::make_tuple(loc, "*",""));
                        break;
                    case 1:
                        board.push_back(std::make_tuple(loc, 
                                    player_one.get_piece_symbol(ROOK),
                                    player_one.get_team()));
                        player_one.set_piece_position(
                                player_one.get_pieces().at(5),loc);
                        break;
                    case 2:
                        board.push_back(std::make_tuple(loc, 
                                    player_one.get_piece_symbol(BISHOP),
                                    player_one.get_team()));
                        player_one.set_piece_position(
                                player_one.get_pieces().at(4),loc);
                        break;
                    case 3:
                        board.push_back(std::make_tuple(loc, 
                                    player_one.get_piece_symbol(KNIGHT),
                                    player_one.get_team()));
                        player_one.set_piece_position(
                                player_one.get_pieces().at(2),loc);
                        break;
                    case 4:
                        board.push_back(std::make_tuple(loc, 
                                    player_two.get_piece_symbol(KNIGHT),
                                    player_two.get_team()));
                        player_two.set_piece_position(
                                player_two.get_pieces().at(2),loc);
                        break;
                    case 5:
                        board.push_back(std::make_tuple(loc, 
                                    player_two.get_piece_symbol(BISHOP),
                                    player_two.get_team()));
                        player_two.set_piece_position(
                                player_two.get_pieces().at(4),loc);
                        break;
                    case 6:
                        board.push_back(std::make_tuple(loc, 
                                    player_two.get_piece_symbol(ROOK),
                                    player_two.get_team()));
                        player_two.set_piece_position(
                                player_two.get_pieces().at(5),loc);
                        break;
                    case 7:
                        board.push_back(std::make_tuple(loc, 
                                    player_one.get_piece_symbol(KING),
                                    player_one.get_team()));
                        player_one.set_piece_position(
                                player_one.get_pieces().at(0),loc);
                        break;
                    case 8:
                        board.push_back(std::make_tuple(loc, 
                                    player_one.get_piece_symbol(BISHOP),
                                    player_one.get_team()));
                        player_one.set_piece_position(
                                player_one.get_pieces().at(3),loc);
                        break;
                    case 9:
                        board.push_back(std::make_tuple(loc, 
                                    player_one.get_piece_symbol(KNIGHT),
                                    player_one.get_team()));
                        player_one.set_piece_position(
                                player_one.get_pieces().at(1),loc);
                        break;
                    case 10:
                        board.push_back(std::make_tuple(loc, 
                                    player_two.get_piece_symbol(KNIGHT),
                                    player_two.get_team()));
                        player_two.set_piece_position(
                                player_two.get_pieces().at(1),loc);
                        break;
                    case 11:
                        board.push_back(std::make_tuple(loc, 
                                    player_two.get_piece_symbol(BISHOP),
                                    player_two.get_team()));
                        player_two.set_piece_position(
                                player_two.get_pieces().at(3),loc);
                        break;
                    case 12:
                        board.push_back(std::make_tuple(loc, 
                                    player_two.get_piece_symbol(KING),
                                    player_two.get_team()));
                        player_two.set_piece_position(
                                player_two.get_pieces().at(0),loc);
                        break;
                    default:
                        break;
                }
            }
        }
    }
    // 97 - 122
    void update_possible_moves_for_player(ChessTeam& team) {
        for (PieceInfo& p : team.get_pieces()) {
            if (p.name == 0) {
                char move_left = p.pos[0] - 1;
                char move_right = p.pos[0] + 1;
                int move_up = atoi(&p.pos[1]) + 1;
                int move_down = atoi(&p.pos[1]) - 1; // int to check for < 0
                std::cout << move_left<<" "<<move_right<<" "<<move_up <<" "<<move_down<<"\n";
                for (uint_t i = 0; i < 4; i++) {p.possible_move[i] = "";}
                if (move_left >= 97) { p.possible_move[0] = 
                    (move_left) + std::to_string(atoi(&p.pos[1]));}
                if (move_right <= 104) { p.possible_move[1] = 
                    (move_right) + std::to_string(atoi(&p.pos[1]));}
                if (move_up >= 1) { p.possible_move[2] = 
                    p.pos[0] + std::to_string(move_up); }
                if (move_down >= 8) { p.possible_move[3] = 
                    p.pos[0] + std::to_string(move_down); }
            }
            if (p.name == 1)
        }
    }
    void print_possible_moves_for_player(ChessTeam& team) {
        for (PieceInfo& p : team.get_pieces()) {
            std::cout << team.get_piece_symbol(p.name) << " can move to: ";
            std::cout << p.possible_move[0] << ", " << p.possible_move[1] << ", ";
            std::cout << p.possible_move[2] << ", "  << p.possible_move[3];
            std::cout << "\n";
        }
    }
    void redraw_board() {
        for (std::tuple<pos_t, piece_t, team_t> n : board) {
            std::cout<<std::get<0>(n)<<" "<<std::get<1>(n)<<" "<<std::get<2>(n);
            std::cout << "\n";
        }
    }
    
    std::vector<std::tuple<pos_t, piece_t, team_t>> print_board() {
        unsigned int max = 0;
        bool flip = true;
        for (std::tuple<pos_t, piece_t, team_t> n : board) {
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

int main() {
    ChessGame chess = ChessGame();
    chess.set_board();
    chess.print_board();

    ChessTeam p1 = chess.get_player(Team::WHITE);
    ChessTeam p2 = chess.get_player(Team::BLACK);
    
    for (PieceInfo p : p1.get_pieces()) {
        std::cout << p.name << " " << p.pos << "\n";
    }
    chess.update_possible_moves_for_player(p1);
    chess.print_possible_moves_for_player(p1);


    p1.take_piece(p2, ROOK);

    return 0;
}
