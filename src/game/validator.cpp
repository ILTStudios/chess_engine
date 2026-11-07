#include <iostream>
#include <cstdint>

#include "validator.h"
#include "board.h"

using namespace std;

// all init piece bitboard moves
void Validator::init_knight_moves(){    
    int rank_offsets[] = { 2,  1, -1, -2, -2, -1,  1,  2 };
    int file_offsets[] = { 1,  2,  2,  1, -1, -2, -2, -1 };

    for(int square = 0; square < 64; square += 1){
        int rank = square / 8;
        int file = square % 8;

        uint64_t moves = 0;

        for(int i = 0; i < 8; i += 1){
            int new_rank = rank + rank_offsets[i];
            int new_file = file + file_offsets[i];

            if(new_rank >= 0 && new_rank < 8  && new_file >= 0 && new_file < 8){
                int target_square = (new_rank * 8) + new_file;
                moves |= (1ULL << target_square);
            }
        }
        knight_moves[square] = moves;
    }
    cout << "Successfully generated all knight moves!" << endl;
}

void Validator::init_king_moves(){
    int rank_offsets[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int file_offsets[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for(int square = 0; square < 64; square += 1){
        int rank = square / 8;
        int file = square % 8;

        uint64_t moves = 0;

        for(int i = 0; i < 8; i += 1){
            int new_rank = rank + rank_offsets[i];
            int new_file = file + file_offsets[i];

            if(new_rank >= 0 && new_rank < 8 && new_file >= 0 && new_file < 8){
                int target_square = (new_rank * 8) + new_file;
                moves |= (1ULL << target_square);
            }
        }

        king_moves[square] = moves;
    }
    cout << "Successfully generated all king moves!" << endl;
}

void Validator::init_pawn_moves(){
    for(int square = 0; square < 64; square += 1){
        uint64_t current_square = (1ULL << square);
        uint64_t w_moves = 0;
        uint64_t b_moves = 0;
        
        if(square >= 8 && square < 16){
            w_moves |= (current_square << 16);
        }
        if(square >= 48 && square < 56){
            b_moves |= (current_square >> 16);
        }

        w_moves |= (current_square << 8);
        uint64_t w_captures = 0;
        if ((current_square & 0xFEFEFEFEFEFEFEFEULL) != 0) w_captures |= (current_square << 7);
        if ((current_square & 0x7F7F7F7F7F7F7F7FULL) != 0) w_captures |= (current_square << 9);
        w_pawn_captures[square] = w_captures;
        w_pawn_moves[square] = w_moves;

        b_moves |= (current_square >> 8);
        uint64_t b_captures = 0;
        if ((current_square & 0xFEFEFEFEFEFEFEFEULL) != 0) b_captures |= (current_square >> 9);
        if ((current_square & 0x7F7F7F7F7F7F7F7FULL) != 0) b_captures |= (current_square >> 7);
        b_pawn_captures[square] = b_captures;
        b_pawn_moves[square] = b_moves;
    }
    cout << "Successfully generated all pawns moves!" << endl;
}

void Validator::init_rook_moves(){
    int rank_offsets[] = {1, -1, 0, 0};
    int file_offsets[] = {0, 0, 1, -1};

    for (int square = 0; square < 64; square += 1){
        int rank = square / 8;
        int file = square % 8;

        uint64_t moves = 0;

        for(int i = 0; i < 4; i += 1){
            int new_rank = rank + rank_offsets[i];
            int new_file = file + file_offsets[i];

            while(new_rank >= 0 && new_rank < 8 && new_file >= 0 && new_file < 8){
                moves |= (1ULL << ((new_rank * 8) + new_file));
                new_rank += rank_offsets[i];
                new_file += file_offsets[i];
            }
        }

        rook_moves[square] = moves;
    }
    cout << "Successfully generated all rook moves!" << endl;
}

void Validator::init_bishop_moves(){
    int rank_offsets[] = {1, 1, -1, -1};
    int file_offsets[] = {1, -1, 1, -1};

    for(int square = 0; square < 64; square += 1){
        int rank = square / 8;
        int file = square % 8;

        uint64_t moves = 0;

        for(int i = 0; i < 4; i += 1){
            int new_rank = rank + rank_offsets[i];
            int new_file = file + file_offsets[i];

            while(new_rank >= 0 && new_rank < 8 && new_file >= 0 && new_file < 8){
                moves |= (1ULL << ((new_rank * 8) + new_file));
                new_rank += rank_offsets[i];
                new_file += file_offsets[i];
            }
        }
        bishop_moves[square] = moves;
    }

    cout << "Sucessfully generated all bishop moves!" << endl;
}

void Validator::initialise(){
    // initialise and produce all the move bitboards
    init_knight_moves();
    init_king_moves();
    init_pawn_moves();
    init_rook_moves();
    init_bishop_moves();
    
    for(int square = 0; square < 64; square += 1){
        uint64_t rook_mask = rook_moves[square];
        uint64_t bishop_mask = bishop_moves[square];

        queen_moves[square] = (rook_mask | bishop_mask);
    }
    //

    // other special rules
    enpassen_square = 0;
    white_castle_ks = true;
    white_castle_qs = true;
    black_castle_ks = true;
    black_castle_qs = true;
    //

    cout << endl;
}

// checks 2 things
// are you trying to attack your own piece?
// are you moving your own piece?
bool is_own_piece(uint64_t from_square, uint64_t to_square, int player_turn, const Board &board){
    // is that your piece?
    // are you attacking your own piece?
    uint64_t own_pieces;
    
    if (player_turn == 1){
        own_pieces = board.get_white_pieces(); 
    }else if(player_turn == 0){
        own_pieces = board.get_black_pieces();
    }else{
        cout << "Internal Variable Player Turn is out of bounds!" << endl;
        return false;
    }

    if (!(from_square & own_pieces)) {
        return false;
    }

    // Cannot capture your own piece
    if (to_square & own_pieces) {
        return false;
    }

    return true;    
}

// checks what kind of piece you are trying to move
char what_piece(uint64_t from_mask, const Board &board){
    if      (board.get_white_pawns() & from_mask)   return 'P';
    else if (board.get_white_knights() & from_mask) return 'N';
    else if (board.get_white_bishops() & from_mask) return 'B';
    else if (board.get_white_rooks() & from_mask)   return 'R';
    else if (board.get_white_queen() & from_mask)   return 'Q';
    else if (board.get_white_king() & from_mask)    return 'K';
    
    else if (board.get_black_pawns() & from_mask)   return 'p';
    else if (board.get_black_knights() & from_mask) return 'n';
    else if (board.get_black_bishops() & from_mask) return 'b';
    else if (board.get_black_rooks() & from_mask)   return 'r';
    else if (board.get_black_queen() & from_mask)   return 'q';
    else if (board.get_black_king() & from_mask)    return 'k';

    return '.'; // Empty square
}

// is_valid_move supplementary functions
bool Validator::is_path_clear(int from_index, int to_index, const Board &board){
    uint64_t occupied = board.get_white_pieces() | board.get_black_pieces();

    int from_rank = from_index / 8;
    int from_file = from_index % 8;

    int to_rank = to_index / 8;
    int to_file = to_index % 8;

    int rank_direction = (to_rank > from_rank) - (to_rank < from_rank);
    int file_direction = (to_file > from_file) - (to_file < from_file);

    int current_rank = from_rank + rank_direction;
    int current_file = from_file + file_direction;

    while (current_rank != to_rank || current_file != to_file) {

        int current_square = current_rank * 8 + current_file;
        uint64_t current_mask = 1ULL << current_square;

        if (occupied & current_mask) {
            return false;
        }

        current_rank += rank_direction;
        current_file += file_direction;
    }

    return true;
}
bool Validator::is_valid_pawn_move(int from_square, uint64_t to_mask, int player_turn, const Board &board){
    uint64_t occupied = board.get_white_pieces() | board.get_black_pieces();

    if (player_turn == 1) {
        if (w_pawn_moves[from_square] & to_mask){
            if(occupied & to_mask) return false;
            if (abs(__builtin_ctzll(to_mask) - from_square) == 16) {
                uint64_t intermediate_mask = 1ULL << (from_square + 8);
                if (occupied & intermediate_mask) return false;
            }
            return true;
        }
        if (w_pawn_captures[from_square] & to_mask) return (board.get_black_pieces() & to_mask) != 0;

    }else if (player_turn == 0) {
        if (b_pawn_moves[from_square] & to_mask){
            if(occupied & to_mask) return false;
            if (abs(__builtin_ctzll(to_mask) - from_square) == 16) {
                uint64_t intermediate_mask = 1ULL << (from_square - 8);
                if (occupied & intermediate_mask) return false;
            }
            return true; 
        } 
        if (b_pawn_captures[from_square] & to_mask) return (board.get_white_pieces() & to_mask) != 0;
    }else cout << "Invalid Internal Variable Player Turn!" << endl;
    
    return false;
}
bool Validator::is_valid_knight_move(int from_square, uint64_t to_mask){
   return (knight_moves[from_square] & to_mask) != 0;
}
bool Validator::is_valid_bishop_move(int from_square, uint64_t to_mask, const Board &board){
    if (!(bishop_moves[from_square] & to_mask)) return false;
    return is_path_clear(from_square, __builtin_ctzll(to_mask), board);
}
bool Validator::is_valid_rook_move(int from_square, uint64_t to_mask, const Board &board){
    if (!(rook_moves[from_square] & to_mask)) return false;
    return is_path_clear(from_square, __builtin_ctzll(to_mask), board);
}
bool Validator::is_valid_king_move(int from_square, uint64_t to_mask){
    return (king_moves[from_square] & to_mask) != 0;
}
bool Validator::is_valid_queen_move(int from_square, uint64_t to_mask, const Board &board){
    if (!(queen_moves[from_square] & to_mask)) return false;
    return is_path_clear(from_square, __builtin_ctzll(to_mask), board);
}
// checks whether that was a valid move according to that piece
bool Validator::is_valid_move(uint64_t from_mask, uint64_t to_mask, int player_turn, const Board &board){
    char piece_type = what_piece(from_mask, board);
    int from_index = __builtin_ctzll(from_mask);

    switch (piece_type) {
        case 'P':
        case 'p':
            return is_valid_pawn_move(from_index, to_mask, player_turn, board);

        case 'N':
        case 'n':
            return is_valid_knight_move(from_index, to_mask);

        case 'B':
        case 'b':
            return is_valid_bishop_move(from_index, to_mask, board);

        case 'R':
        case 'r':
            return is_valid_rook_move(from_index, to_mask, board);

        case 'Q':
        case 'q':
            return is_valid_queen_move(from_index, to_mask, board);

        case 'K':
        case 'k':
            return is_valid_king_move(from_index, to_mask);

        default:
            return false;
    }
}

bool Validator::is_valid(int player_turn, int from_square, int to_square, const Board &board){
    uint64_t from_mask = (1ULL << from_square);
    uint64_t to_mask = (1ULL << to_square);

    // is that your piece?
    if(!is_own_piece(from_mask, to_mask, player_turn, board)){
        cout << "You are either moving an enemy piece, or attacking your own!" << endl;
        return 0;
    }
    //

    // checks whether that was a valid move according to what piece it is
    if(!is_valid_move(from_mask, to_mask, player_turn, board)){
        cout << "That is not a valid move! Try again" << endl;
        return 0;
    }

    
    return 1;
}