#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <vector>
#include <tuple>
#include <map>
#include <cassert>

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
using name_t = std::string;
using uint_t = unsigned int;
std::string nbs = " ";

enum State {UNFINISHED, WHITE_WON, BLACK_WON};
enum Team {WHITE, BLACK};
enum Piece {KING,KNIGHT,BISHOP,ROOK};
struct PieceInfo {
    Team team;
    Piece name;
    piece_t symbol;
    pos_t pos;
    std::vector<pos_t> possible_moves;
};
struct BoardItem {
    int index;
    pos_t pos;
    PieceInfo* piece;
};

class ChessTeam {
    private:
    std::map<Team, team_t> team_map {
        {Team::WHITE, "WHITE"},
        {Team::BLACK, "BLACK"}
    };
    std::map<Piece, team_t> symbol_map;
    std::map<Piece, name_t> name_map;
    std::string team;
    std::vector<PieceInfo> pieces;

    public:
    ChessTeam(Team t) : team(team_map[t]) {
        name_map = {
            {KING,"KING"},{KNIGHT,"KNIGHT"},{BISHOP,"BISHOP"},{ROOK,"ROOK"}
        };
        if (team == "WHITE") {
            symbol_map = {
                {Piece::KING, KING_WHITE},
                {Piece::KNIGHT, KNIGHT_WHITE},
                {Piece::ROOK, ROOK_WHITE},
                {Piece::BISHOP, BISHOP_WHITE}
            };
            // team, name, symbol, position, possible moves
            pieces = {
                PieceInfo{Team::WHITE,KING,KING_WHITE,"",{""}},
                PieceInfo{Team::WHITE,KNIGHT,KNIGHT_WHITE,"",{""}},
                PieceInfo{Team::WHITE,KNIGHT,KNIGHT_WHITE,"",{""}},
                PieceInfo{Team::WHITE,BISHOP,BISHOP_WHITE,"",{""}},
                PieceInfo{Team::WHITE,BISHOP,BISHOP_WHITE,"",{""}},
                PieceInfo{Team::WHITE,ROOK,ROOK_WHITE,"",{""}}
            };

        } else {
            symbol_map = {
                {Piece::KING, KING_BLACK},
                {Piece::KNIGHT, KNIGHT_BLACK},
                {Piece::ROOK, ROOK_BLACK},
                {Piece::BISHOP, BISHOP_BLACK}
            };
            pieces = {
                PieceInfo{Team::BLACK,KING,KING_BLACK,"",{""}},
                PieceInfo{Team::BLACK,KNIGHT,KNIGHT_BLACK,"",{""}},
                PieceInfo{Team::BLACK,KNIGHT,KNIGHT_BLACK,"",{""}},
                PieceInfo{Team::BLACK,BISHOP,BISHOP_BLACK,"",{""}},
                PieceInfo{Team::BLACK,BISHOP,BISHOP_BLACK,"",{""}},
                PieceInfo{Team::BLACK,ROOK,ROOK_BLACK,"",{""}}
            };
        }
    } 
    ~ChessTeam() {}
    //friend class ChessGame;
    std::string get_team() {
        return team;
    }

    std::vector<PieceInfo>& get_pieces() {
        return pieces;
    }

    piece_t get_piece_symbol(Piece p) {
        return symbol_map[p];
    }

    piece_t get_piece_name(Piece p) {
        return name_map[p];
    }

    void set_piece_position(PieceInfo* p, pos_t pos) {
        p->pos = pos;
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

    void remove_piece(pos_t del) {
        uint i = 0;
        for (PieceInfo& p : pieces) {
            if (p.pos == del) {
                cout " removing piece: " << p.pos endl;
                pieces.erase(pieces.begin()+i);
                break;
            }
            i++;
        }
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
    std::vector<BoardItem> board;
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
    piece_t get_piece_name(ChessTeam& team, Piece p) {
        return team.get_piece_name(p);
    }

    void set_board() {
        int i = 0;
        for (int row = size - 1; row >= 0; row--) {
            for (uint_t col = 0; col < size; col++) {
                BoardItem item;
                std::string loc = alpha8[col] + std::to_string(num8[row]);
                item.pos = loc; item.index = i++; 
                switch (starting_location[loc]) {
                    case 0:
                        item.piece = nullptr;
                        board.push_back(item);
                        break;
                    case 1:
                        item.piece = &player_one.get_pieces().at(5);
                        player_one.set_piece_position(item.piece,loc);
                        board.push_back(item);
                        break;
                    case 2:
                        item.piece = &player_one.get_pieces().at(4);
                        player_one.set_piece_position(item.piece,loc);
                        board.push_back(item);
                        break;
                    case 3:
                        item.piece = &player_one.get_pieces().at(2);
                        player_one.set_piece_position(item.piece,loc);
                        board.push_back(item);
                        break;
                    case 4:
                        item.piece = &player_two.get_pieces().at(2);
                        player_two.set_piece_position(item.piece,loc);
                        board.push_back(item);
                        break;
                    case 5:
                        item.piece = &player_two.get_pieces().at(4);
                        player_two.set_piece_position(item.piece,loc);
                        board.push_back(item);
                        break;
                    case 6:
                        item.piece = &player_two.get_pieces().at(5);
                        player_two.set_piece_position(item.piece,loc);
                        board.push_back(item);
                        break;
                    case 7:
                        item.piece = &player_one.get_pieces().at(0);
                        player_one.set_piece_position(item.piece,loc);
                        board.push_back(item);
                        break;
                    case 8:
                        item.piece = &player_one.get_pieces().at(3);
                        player_one.set_piece_position(item.piece,loc);
                        board.push_back(item);
                        break;
                    case 9:
                        item.piece = &player_one.get_pieces().at(1);
                        player_one.set_piece_position(item.piece,loc);
                        board.push_back(item);
                        break;
                    case 10:
                        item.piece = &player_two.get_pieces().at(1);
                        player_two.set_piece_position(item.piece,loc);
                        board.push_back(item);
                        break;
                    case 11:
                        item.piece = &player_two.get_pieces().at(3);
                        player_two.set_piece_position(item.piece,loc);
                        board.push_back(item);
                        break;
                    case 12:
                        item.piece = &player_two.get_pieces().at(0);
                        player_two.set_piece_position(item.piece,loc);
                        board.push_back(item);
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
            char l = left; int u = up;
            while (l != 'a'-1) {
                p.possible_moves.push_back((l)+std::to_string(u));
                l--; u++;
            }
        }

        // right+up
        if (right <= 104 && up <= 8) { 
            char r = right; int u = up;
            while (r != 'h'+1) {
                p.possible_moves.push_back((r)+std::to_string(u));
                r++; u++;
            }
        }
        // down+left
        if (down >= 1 && left != 'a'-1) { 
            int d = down; char l = left;
            while (d >= 1) {
                p.possible_moves.push_back(l + std::to_string(d)); 
                d--; l--;
            }
        }
        // down+right
        if (down >= 1 && right != 'h'+1) {
            int d = down; char r = right;
            while (d >= 1) {
                p.possible_moves.push_back(r + std::to_string(d));
                d--;
                r++;
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

    void redraw_board() {
        int i = 0;
        bool blank;
        board.clear();
        for (int row = size - 1; row >= 0; row--) {
            for (uint_t col = 0; col < size; col++) {
                blank = true;
                BoardItem item;
                std::string loc = alpha8[col] + std::to_string(num8[row]);
                item.index = i++; 
                // yeah its wasteful but saves lines
                for (PieceInfo& p : player_one.get_pieces()){
                    if (loc == p.pos) {
                        item.piece = &p;
                        //player_one.set_piece_position(item.piece,loc);
                        board.push_back(item);
                        blank = false;
                    }
                } 
                for (PieceInfo& p : player_two.get_pieces()){
                    if (loc == p.pos) {
                        item.piece = &p;
                        //player_two.set_piece_position(item.piece,loc);
                        board.push_back(item);
                        blank = false;
                    }
                } 
                if (blank) {
                    item.piece = nullptr;
                    board.push_back(item);
                }
            }
        }
        print_board();
    }

    std::vector<BoardItem> print_board() {
        uint_t max = 0, lnum = 8;
        bool flip = true;
        cout "\n\n\t   ";
        for (size_t i = 0; i < 8; i++) {
            cout alpha8[i] << " ";
        }
        cout " \n\t " << lnum << " ";
        for (BoardItem& item : board) {
            piece_t value;
            value = (!item.piece ? " " : item.piece->symbol);
            if (!flip) {
                cout BOX << value << " " << COLOR_RESET;
            } else {
                cout value << " ";
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
   
    // PieceInfo
    /*
    Team team;
    Piece name;
    piece_t symbol;
    pos_t pos;
    std::vector<pos_t> possible_moves;
    */
    BoardItem* find_board_item(pos_t pos) {
        for (BoardItem& item : board) {
            if (item.pos == pos) {
                return &item;
            }
        }
        return nullptr;
    }

    bool path_clear(BoardItem* curr, BoardItem* next) {
        bool clear = false;
        int i = 63-(63-curr->index);
        int j = 63-(63-next->index);
        if (board.at(i).piece->name == 1) {
            return true;
        }
        if (i < j) {
            while (i <= j) {
                if (board.at(i).piece) {
                    for (pos_t p : board.at(i).piece->possible_moves) {
                        if (p != board.at(j).pos && i != j) {
                            clear = false;
                            break;
                        }
                    }
                }
                clear = true;
                i++;
            }
        } else {
            while (i >= j) {
                if (board.at(i).piece) {
                    for (pos_t p : board.at(i).piece->possible_moves) {
                        if (p != board.at(j).pos && i != j) {
                            clear = false;
                            break;
                        }
                    }
                }
                clear = true;
                i--;
            }

        }
        return clear; 
    }

    bool potential_move(PieceInfo* piece, pos_t move) {
        bool found = false;
        for (pos_t& pos : piece->possible_moves) {
            if (pos == move) {
                found = true;
                break;
            }
        }
        return found;
    }

    // returns 0 on successful move, < 0 if piece not found
    int move_piece(Team team, pos_t curr_pos, pos_t next_pos) {
        int ret = -1;
        if (team == Team::WHITE) {
            update_possible_moves_for_player(player_one);
            BoardItem* item = find_board_item(curr_pos);
            BoardItem* next = find_board_item(next_pos);
            if (item && next && potential_move(item->piece, next_pos)) {
                cout "try to move piece: " << get_piece_name(player_one,
                                                        item->piece->name);
                cout " " << item->piece->symbol;
                cout " at pos " << item->pos << " to " << next_pos endl;
                if (path_clear(item,next)) {
                    //redraw_board();
                    /*
                    cout "update" endl; 
                    if (next->piece && next->piece->team != team) {
                        cout " remove piece\n";
                        player_two.remove_piece(next->piece->pos);
                        item->piece->pos = next->piece->pos;
                        item->piece->pos = next->piece->pos;
                        for (PieceInfo& p : player_two.get_pieces()){
                            cout p.pos << " ";
                        }
                        cout nl;
                        player_two.remove_piece(next->piece->pos);
                        for (PieceInfo& p : player_two.get_pieces()){
                            cout p.pos << " ";
                        }
                        ret = 0;
                    }
                    */
                    if (next->piece && next->piece->team == team) {
                        ret = -1;
                    }
                    if (!next->piece) {
                        item->piece->pos = next->pos;
                        ret = 0;
                    }
                    redraw_board();
                }
                ret = 0; 
            } else {ret = -1;}
        }

        if (team == Team::BLACK) {
            cout "\tfind player two piece at pos " << curr_pos endl;
        }
        return ret;
    }
};

int main() {
    ChessGame chess = ChessGame();
    chess.set_board();
    chess.print_board();

    //chess.move_piece(Team::WHITE,"a2","d2");// invalid move
    chess.move_piece(Team::WHITE,"b2","c3");// valid move
    chess.move_piece(Team::WHITE,"c3","d4");// valid move
    //chess.move_piece(Team::WHITE,"a3","c5");// valid move
    //chess.move_piece(Team::WHITE,"c5","f2");// valid move




    //chess.move_piece(Team::WHITE,"a2","a3");// valid move
    chess.move_piece(Team::BLACK,"f2","d3");// valid move
    chess.move_piece(Team::BLACK,"f2","h2");// invalid move
    return 0;
}
