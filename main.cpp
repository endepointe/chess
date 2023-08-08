#include <stdexcept>
#include <iostream>
#include <algorithm>
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
#define cout            std::cout<<
#define nl              "\n"
#define endl            <<nl

using piece_t = std::string; 
using pos_t = std::string;
using team_t = std::string;
using uint_t = unsigned int;
// idk if ill need this
/*
enum Pieces {
    KING_WHITE,KNIGHT_WHITE,BISHOP_WHITE,ROOK_WHITE,
    KING_BLACK,KNIGHT_BLACK,BISHOP_BLACK,ROOK_BLACKE,
};
*/
enum State {UNFINISHED, WHITE_WON, BLACK_WON};
enum Team {WHITE, BLACK};
enum Piece {KING,KNIGHT,BISHOP,ROOK};
struct PieceInfo {
    Team team;
    Piece name;
    pos_t pos;
    std::vector<pos_t> possible_moves;
};


class ChessTeam {
    private:
    std::map<Team, team_t> team_map {
        {Team::WHITE, "WHITE"},
        {Team::BLACK, "BLACK"}
    };
    std::map<Piece, team_t> piece_map;
    std::string team;
    std::vector<PieceInfo> pieces;
    /*{
        PieceInfo{"",KING,"",{""}},PieceInfo{"",KNIGHT,"",{""}},
        PieceInfo{"",KNIGHT,"",{""}},PieceInfo{"",BISHOP,"",{""}},
        PieceInfo{"",BISHOP,"",{""}},PieceInfo{"",ROOK,"",{""}},
    };
    */
    public:
    ChessTeam(Team t) : team(team_map[t]) {
        if (team == "WHITE") {
            piece_map = {
                {Piece::KING, KING_WHITE},
                {Piece::KNIGHT, KNIGHT_WHITE},
                {Piece::ROOK, ROOK_WHITE},
                {Piece::BISHOP, BISHOP_WHITE}
            };
            pieces = {
                PieceInfo{Team::WHITE,KING,"",{""}},
                PieceInfo{Team::WHITE,KNIGHT,"",{""}},
                PieceInfo{Team::WHITE,KNIGHT,"",{""}},
                PieceInfo{Team::WHITE,BISHOP,"",{""}},
                PieceInfo{Team::WHITE,BISHOP,"",{""}},
                PieceInfo{Team::WHITE,ROOK,"",{""}}
            };

        } else {
            piece_map = {
                {Piece::KING, KING_BLACK},
                {Piece::KNIGHT, KNIGHT_BLACK},
                {Piece::ROOK, ROOK_BLACK},
                {Piece::BISHOP, BISHOP_BLACK}
            };
            pieces = {
                PieceInfo{Team::BLACK,KING,"",{""}},
                PieceInfo{Team::BLACK,KNIGHT,"",{""}},
                PieceInfo{Team::BLACK,KNIGHT,"",{""}},
                PieceInfo{Team::BLACK,BISHOP,"",{""}},
                PieceInfo{Team::BLACK,BISHOP,"",{""}},
                PieceInfo{Team::BLACK,ROOK,"",{""}}
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

    pos_t get_piece_position(PieceInfo& p) {
        return p.pos;
    }

    PieceInfo* get_piece_at_position(pos_t pos){
        for (PieceInfo& p : pieces){
            if (p.pos == pos) {
                return &p;
            }
        }
        return nullptr;
    }

    void move_piece(pos_t curr_pos, pos_t next_pos) {
        PieceInfo* piece_ptr = nullptr;
        for (PieceInfo& p : pieces) {
            if (p.pos == curr_pos) {
                // check if next_pos is open
                piece_ptr = get_piece_at_position(next_pos);
                cout piece_ptr->pos << " contains ";
                cout team_map[piece_ptr->team] << " Race stinks " endl;
            }
        }
    }

    void take_piece(ChessTeam &other_team, Piece p) {
        cout "Team: " << get_team();
        cout " takes:" << other_team.get_team();
        cout "'s " << other_team.get_piece_symbol(p) << "\n";
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
    uint_t num8[8] = {1,2,3,4,5,6,7,8};
    uint_t size = 8;
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
    ChessTeam& get_player(Team t) {
        if (t == 0) {
            return player_one;
        }
        return player_two;
    }

    void set_board() {
        for (int row = size - 1; row >= 0; row--) {
            for (uint_t col = 0; col < size; col++) {
                std::string loc = alpha8[col] + std::to_string(num8[row]);
                switch (starting_location[loc]) {
                    case 0:
                        board.push_back(std::make_tuple(loc, " ",""));
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

    void update_moves_for_king(PieceInfo& p) {
        char left = p.pos[0] - 1;
        char right = p.pos[0] + 1;
        int up = atoi(&p.pos[1]) + 1;
        int down = atoi(&p.pos[1]) - 1; // int to check for < 0
        p.possible_moves.clear();
        if (left >= 97) { 
            p.possible_moves.push_back((left) + std::to_string(atoi(&p.pos[1])));
        }
        if (right <= 104) { 
            p.possible_moves.push_back((right) + std::to_string(atoi(&p.pos[1])));
        }
        if (up >= 1) { 
            p.possible_moves.push_back(p.pos[0] + std::to_string(up)); 
        }
        if (down >= 8) {
            p.possible_moves.push_back(p.pos[0] + std::to_string(down));
        }
        if (up <= 8 && left >= 97) {
            p.possible_moves.push_back(left + std::to_string(up));
        }
        if (up <= 8 && right <= 104) {
            p.possible_moves.push_back(right + std::to_string(up));
        }
        if (down >= 1 && right <= 104) {
            p.possible_moves.push_back(right + std::to_string(down));
        }
        if (down >= 1 && left >= 97) {
            p.possible_moves.push_back(left + std::to_string(down));
        }
    }
    void update_moves_for_knight(PieceInfo& p) {
        int up2 = atoi(&p.pos[1]) + 2;
        int down2 = atoi(&p.pos[1]) - 2;
        int up1 = atoi(&p.pos[1]) + 1;
        int down1 = atoi(&p.pos[1]) - 1;
        char left1 = p.pos[0] - 1;
        char right1 = p.pos[0] + 1;
        char left2 = p.pos[0] - 2;
        char right2 = p.pos[0] + 2;

        p.possible_moves.clear();

        if (up2 <= 8 && left1 >= 97) {
            p.possible_moves.push_back(left1 + std::to_string(up2));
        }
        if (up2 <= 8 && right1 <= 104) {
            p.possible_moves.push_back(right1 + std::to_string(up2));
        }
        if (down2 >= 1 && left1 >= 97) {
            p.possible_moves.push_back(left1 + std::to_string(down2));
        }
        if (down2 >= 1 && right1 <= 104) {
            p.possible_moves.push_back(right1 + std::to_string(down2));
        }
        if (up1 <= 8 && left2 >= 97) {
            p.possible_moves.push_back(left2 + std::to_string(up1));
        }
        if (down1 >= 1 && left2 >= 97) {
            p.possible_moves.push_back(left2 + std::to_string(down1));
        }
        if (up1 <= 8 && right2 <= 104) {
            p.possible_moves.push_back(right2 + std::to_string(up1));
        }
        if (down1 >= 1 && right2 <= 104) {
            p.possible_moves.push_back(right2 + std::to_string(down1));
        }
    }
    void update_moves_for_bishop(PieceInfo& p) {
        char left = p.pos[0] - 1;
        char right = p.pos[0] + 1;
        int up = atoi(&p.pos[1]) + 1;
        int down = atoi(&p.pos[1]) - 1; // int to check for < 0
        p.possible_moves.clear();
        // left+up
        if (left >= 97 && up <= 8) { 
            while (left != 'a'-1) {
                p.possible_moves.push_back(
                        (left)+std::to_string(up));
                left--;
                up++;
            }
        }
        // right+up
        if (right <= 122 && up <= 8) { 
            while (right != 'h'+1) {
                p.possible_moves.push_back(
                        (right)+std::to_string(up));
                right++;
                up++;
            }
        }
        // down+left
        if (down >= 1 && left != 'a'-1) { 
            while (down >= 1) {
                p.possible_moves.push_back(left + std::to_string(down)); 
                down--;
                left--;
            }
        }
        // down+right
        if (down >= 1 && right != 'h'+1) {
            while (down >= 1) {
                p.possible_moves.push_back(right + std::to_string(down));
                down--;
                right++;
            }
        }

    }
    void update_moves_for_rook(PieceInfo& p) {
        char left = p.pos[0] - 1;
        char right = p.pos[0] + 1;
        int up = atoi(&p.pos[1]) + 1;
        int down = atoi(&p.pos[1]) - 1; // int to check for < 0
        p.possible_moves.clear();
        if (left >= 97) { 
            while (left != 'a'-1) {
                p.possible_moves.push_back(
                        (left)+std::to_string(atoi(&p.pos[1])));
                left--;
            }
        }
        if (right <= 122) { 
            while (right != 'h'+1) {
                p.possible_moves.push_back(
                        (right)+std::to_string(atoi(&p.pos[1])));
                right++;
            }
        }
        if (up >= 1) { 
            while (up <= 8) {
                p.possible_moves.push_back(p.pos[0] + std::to_string(up)); 
                up++;
            }
        }
        if (down >= 8) {
            while (down >= 1) {
                p.possible_moves.push_back(p.pos[0] + std::to_string(down));
                down--;
            }
        }
    }
    // 97 - 104
    void update_possible_moves_for_player(ChessTeam& team) {
        for (PieceInfo& p : team.get_pieces()) {
            if (p.name == 0) {
                update_moves_for_king(p);
            }
            // knight
            if (p.name == 1) { 
                update_moves_for_knight(p);
            }
            // bishop
            if (p.name == 2) { 
                update_moves_for_bishop(p);
            }
            // rooks
            if (p.name == 3) { 
                update_moves_for_rook(p);
            }
        }
    }
    void print_possible_moves_for_player(ChessTeam& team) {
        for (PieceInfo& p : team.get_pieces()) {
            cout team.get_piece_symbol(p.name);
            cout " at position " << team.get_piece_position(p) << " can move to: ";
            for (pos_t m : p.possible_moves) {
                cout m << " ";
            }
            cout "\n";
        }
        cout "\n";
    }
    void redraw_board() {
        for (std::tuple<pos_t, piece_t, team_t> n : board) {
            cout std::get<0>(n)<<" "<<std::get<1>(n)<<" "<<std::get<2>(n);
            cout "\n";
        }
    }
    
    std::vector<std::tuple<pos_t, piece_t, team_t>> print_board() {
        uint_t max = 0, lnum = 8;
        bool flip = true;
        cout "\n\n\t   ";
        for (size_t i = 0; i < 8; i++) {
            cout alpha8[i] << " ";
        }
        cout " \n\t " << lnum << " ";
        for (std::tuple<pos_t, piece_t, team_t> n : board) {
            if (!flip) {
                cout BOX << std::get<1>(n) << " " << COLOR_RESET;
            } else {
                cout std::get<1>(n) << " ";
            }
            flip = !flip;
            if (max == 7) {
                if (lnum - 1 == 0) {
                    cout lnum << " \n\t " << "  ";
                } else {
                    cout lnum << " \n\t " << lnum-1 << " ";
                }
                flip = !flip;
                max = 0;
                lnum--;
            } else {
                max++;
            }
        }
        for (size_t i = 0; i < 8; i++) {
            cout alpha8[i] << " ";
        }
        cout "\n\n\n";
        return board;
    }
};

int main() {
    ChessGame chess = ChessGame();
    chess.set_board();
    chess.print_board();

    ChessTeam p1 = chess.get_player(Team::WHITE);
    ChessTeam p2 = chess.get_player(Team::BLACK);
    
    chess.update_possible_moves_for_player(p1);
    chess.update_possible_moves_for_player(p2);

    chess.print_possible_moves_for_player(p1);
    chess.print_possible_moves_for_player(p2);

    p1.take_piece(p2, ROOK);
    p1.move_piece("a2","b2");

    return 0;
}
