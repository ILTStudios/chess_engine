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
    // cout << "Successfully generated all knight moves!" << endl;
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
    // cout << "Successfully generated all king moves!" << endl;
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
    // cout << "Successfully generated all pawns moves!" << endl;
}
void Validator::init_rook_moves(){
    int rank_offsets[] = {1, -1, 0, 0};
    int file_offsets[] = {0, 0, 1, -1};

    for (int square = 0; square < 64; square += 1){
        int rank = square / 8;
        int file = square % 8;

        uint64_t moves = 0;
        uint64_t masks = 0;

        for(int i = 0; i < 4; i += 1){
            int new_rank = rank + rank_offsets[i];
            int new_file = file + file_offsets[i];

            while(new_rank >= 0 && new_rank < 8 && new_file >= 0 && new_file < 8){
                if (new_rank > 0 && new_rank < 7 && new_file > 0 && new_file < 7)
                {
                    masks |= (1ULL << (new_rank * 8 + new_file)); // purely for the magic bitboard
                }
                
                moves |= (1ULL << ((new_rank * 8) + new_file));
                new_rank += rank_offsets[i];
                new_file += file_offsets[i];
            }
        }
        rook_masks[square] = masks;
        rook_moves[square] = moves;
    }


}
void Validator::init_bishop_moves(){
    int rank_offsets[] = {1, 1, -1, -1};
    int file_offsets[] = {1, -1, 1, -1};

    for(int square = 0; square < 64; square += 1){
        int rank = square / 8;
        int file = square % 8;

        uint64_t moves = 0;
        uint64_t masks = 0;

        for(int i = 0; i < 4; i += 1){
            int new_rank = rank + rank_offsets[i];
            int new_file = file + file_offsets[i];

            while(new_rank >= 0 && new_rank < 8 && new_file >= 0 && new_file < 8){
                if (new_rank > 0 && new_rank < 7 && new_file > 0 && new_file < 7)
                {
                    masks |= (1ULL << (new_rank * 8 + new_file)); // purely for the magic bitboards
                }
                moves |= (1ULL << ((new_rank * 8) + new_file));
                new_rank += rank_offsets[i];
                new_file += file_offsets[i];
            }
        }
        bishop_masks[square] = masks;
        bishop_moves[square] = moves;
    }

    // cout << "Sucessfully generated all bishop moves!" << endl;
}
//

// magic bitboard
uint64_t Validator::set_occupancy(int index, int bits_in_mask, uint64_t mask){
    uint64_t occupancy = 0;
    for (int count = 0; count < bits_in_mask; count++) {
        int square = __builtin_ctzll(mask);
        mask &= mask - 1;
        if (index & (1 << count)) {
            occupancy |= (1ULL << square);
        }
    }
    return occupancy;
}
// 

// initialises the validator class
void Validator::initialise(){
    // initialise and produce all the move bitboards
    init_knight_moves();
    init_king_moves();
    init_pawn_moves();
    init_rook_moves();
    init_bishop_moves();
        // initialising the queen moves
    for(int square = 0; square < 64; square += 1){
        uint64_t rook_mask = rook_moves[square];
        uint64_t bishop_mask = bishop_moves[square];

        queen_moves[square] = (rook_mask | bishop_mask);
    }
        //
    //

    // other special rules
    enpassen_square = -1;
    white_castle_ks = true;
    white_castle_qs = true;
    black_castle_ks = true;
    black_castle_qs = true;
    //
}
//

// is_valid_move supplementary functions
    // checks whether your path is clear or not
bool Validator::is_path_clear(int from_index, int to_index, const Board &board){
    uint64_t occupied = board.get_white_pieces() | board.get_black_pieces(); // bitboard of all occupied pieces

    int from_rank = from_index / 8; 
    int from_file = from_index % 8; 

    int to_rank = to_index / 8; 
    int to_file = to_index % 8; 

    int rank_direction = (to_rank > from_rank) - (to_rank < from_rank); // use comparisions to figure out what rank direction (+1, -1, 0)
    int file_direction = (to_file > from_file) - (to_file < from_file); // use comparisions to figure out what file directions (+1, -1, 0)

    int current_rank = from_rank + rank_direction; // starts us off 
    int current_file = from_file + file_direction; // starts us off

    while (current_rank != to_rank || current_file != to_file) {
        int current_square = current_rank * 8 + current_file; // get the current square we are scanning
        uint64_t current_mask = 1ULL << current_square; // create a mask for that
        if (occupied & current_mask) { // if the mask overlaps the occupied bitboard, return false
            // cout << "The Path was not clear!\n";
            return false;
        }
        
        current_rank += rank_direction; // update the next rank
        current_file += file_direction; // update the next file
    }
    return true;
}
    //

    // checks whether a castle is valid
bool Validator::is_valid_castle(int from_square, int to_square, int player_turn, const Board &board){
    bool white = (player_turn == 1); // are we white?
    int king_start = white ? 4 : 60; // if so our king should be here

    // check whether king is on king start square
    if (from_square != king_start) return false;
    // check if the king is currently in check
    if(is_in_check(board, player_turn)) return  false;

    int rook_square;
    int king_to_square;
    int destination_square;
    int opponent = white ? 0 : 1;

    if(to_square == king_start + 2){
        if (white ? !white_castle_ks : !black_castle_ks) return false;

        rook_square = white ? 7 : 63;
        king_to_square = king_start + 1;
        destination_square = king_start + 2;

        uint64_t occupied = board.get_white_pieces() | board.get_black_pieces();

        if (occupied & (1ULL << king_to_square)) return false;
        if (occupied & (1ULL << destination_square)) return false;
    }
    else if(to_square == king_start - 2){
        if (white ? !white_castle_qs : !black_castle_qs) return false;

        rook_square = white ? 0 : 56;
        king_to_square = king_start - 1;
        destination_square = king_start - 2;

        uint64_t occupied = board.get_white_pieces() | board.get_black_pieces();

        // if queenside, then b file gotta be empty
        int extra_square = king_start - 3;

        if (occupied & (1ULL << extra_square)) return false;
        if (occupied & (1ULL << king_to_square)) return false;
        if (occupied & (1ULL << destination_square)) return false;
    }
    else{
        return false;
    }

    uint64_t rook_mask = 1ULL << rook_square; // get the rook mask

    if (white) {
        if (!(board.get_white_rooks() & rook_mask)) return false; // is the rook actually there?
    }
    else {
        if (!(board.get_black_rooks() & rook_mask)) return false; // same question answered here
    }

    // does king pass through attacked square
    Board temporary_board = board;
    temporary_board.move_piece(from_square, king_to_square);

    if (is_square_attacked(king_to_square, opponent, temporary_board)) return false;

    temporary_board = board;
    temporary_board.move_piece(from_square, destination_square);

    if (is_square_attacked(destination_square, opponent, temporary_board)) return false;

    return true;
}
    //

    // upate all casteling rights
void Validator::update_castleing_rights(int from_square, int to_square){
    // if white king moves
    if (from_square == 4) {
        white_castle_ks = false;
        white_castle_qs = false;
    }

    // if black king moves
    if (from_square == 60) {
        black_castle_ks = false;
        black_castle_qs = false;
    }

    // if white rooks move
    if (from_square == 0) white_castle_qs = false;
    if (from_square == 7) white_castle_ks = false;

    // if black rooks move
    if (from_square == 56) black_castle_qs = false;
    if (from_square == 63) black_castle_ks = false;

    // if a rook was captured
    if (to_square == 0) white_castle_qs = false;
    if (to_square == 7) white_castle_ks = false;
    if (to_square == 56) black_castle_qs = false;
    if (to_square == 63) black_castle_ks = false;
}
    //

    // piece by piece breakdown of whether a move is a valid pnbqrk move or not
bool Validator::is_valid_pawn_move(int from_square, uint64_t to_mask, int player_turn, const Board &board){
    uint64_t occupied = board.get_white_pieces() | board.get_black_pieces(); // get all occupied bits

    // get the right movement data
    uint64_t pawn_moves = (player_turn == 1) ? w_pawn_moves[from_square] : b_pawn_moves[from_square];
    uint64_t pawn_captures = (player_turn == 1) ? w_pawn_captures[from_square] : b_pawn_captures[from_square];
    uint64_t enemy_pieces = (player_turn == 1) ? board.get_black_pieces() : board.get_white_pieces();
    // 

    // normal movement up and captures
    if(pawn_moves & to_mask){ // if it is a normal pawn move
        if(occupied & to_mask) return false; // that piece is currently occupied
        int direction = (player_turn == 1) ? 8 : -8; // figure out if we're advancing forward or backwards
        if(to_mask == (1ULL << (from_square + 2 * direction))){
            uint64_t intermediate = (1ULL << (from_square + direction));
            if (occupied & intermediate) return false;
        }
        return true;
    }
    //

    // en passen
    if(to_mask == (1ULL << enpassen_square) && enpassen_square >= 0 && (pawn_captures & to_mask)){
        if (occupied & to_mask) return false;
        
        int direction = (player_turn == 1) ? 8 : -8;
        int captured_square = __builtin_ctzll(to_mask) - direction;

        uint64_t captured_mask = (1ULL << captured_square);

        if(player_turn == 1) return (board.get_black_pieces() & captured_mask) != 0;
        return (board.get_white_pieces() & captured_mask) != 0;
    }
    //

    if(pawn_captures & to_mask) return (enemy_pieces & to_mask) != 0; // if capturing

    return false;
}
bool Validator::is_valid_knight_move(int from_square, uint64_t to_mask){return (knight_moves[from_square] & to_mask) != 0;}
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
bool Validator::is_valid_queen_move(int from_square, uint64_t to_mask, const Board& board){
    if (!(queen_moves[from_square] & to_mask)) return false;
    return is_path_clear(from_square, __builtin_ctzll(to_mask), board);
}
    //
//

// checks whether a square is attacked (required for is in check)
bool Validator::is_square_attacked(int square, int player_turn, const Board &board){
    uint64_t attack_mask = (1ULL << square); // a mask of the piece that is attacked 
    uint64_t attacking_pieces = (player_turn == 1) ? board.get_white_pieces() : board.get_black_pieces(); // get what piece is attacking

    while(attacking_pieces){
        int from_square = __builtin_ctzll(attacking_pieces); // check the last piece
        uint64_t from_mask = (1ULL << from_square); // make a mask
        char piece = board.get_piece(from_mask); // figure out what piece it is
        // iterate over each piece type and figure out if that attack is true or not
        switch (piece)
        {
            case 'P':
                if (w_pawn_captures[from_square] & attack_mask) return true;
                break;

            case 'p':
                if (b_pawn_captures[from_square] & attack_mask) return true;
                break;

            case 'N':
            case 'n':
                if (is_valid_knight_move(from_square, attack_mask)) return true;
                break;

            case 'B':
            case 'b':
                if (is_valid_bishop_move(from_square, attack_mask, board)) return true;
                break;

            case 'R':
            case 'r':
                if (is_valid_rook_move(from_square, attack_mask, board)) return true;
                break;

            case 'Q':
            case 'q':
                if (is_valid_queen_move(from_square, attack_mask, board)) return true;
                break;

            case 'K':
            case 'k':
                if (is_valid_king_move(from_square, attack_mask)) return true;
                break;
        }
        //
        attacking_pieces &= attacking_pieces - 1; // go to the next attacking piece
    }

    return false;
}
//

// checks whether you are in check or not (required for has_legal_moves)
bool Validator::is_in_check(const Board &board, int player_turn){
    uint64_t king = (player_turn == 1) ? board.get_white_king() : board.get_black_king(); // gets your king
    if(king == 0) return false; // not in check
    int king_square = __builtin_ctzll(king); // produce a king square and set a value not on board
    int opponent = (player_turn == 1) ? 0 : 1; // get the opponent
    return is_square_attacked(king_square, opponent, board); // if the opponent is attacking you, you are in check
}
//

// checks whether you have any legal moves (required for checkmate and stalemate check)
bool Validator::has_legal_moves(const Board &board, int player_turn){
    uint64_t own_pieces = (player_turn == 1) ? board.get_white_pieces() : board.get_black_pieces();
    while(own_pieces){
        int from_square = __builtin_ctzll(own_pieces);
        for(int to_square = 0; to_square < 64; to_square += 1){
            if(from_square == to_square) continue; // we are on the same square
            if(!is_valid(player_turn, from_square, to_square, board)) continue; // that kind of movement is not valid
            Board temporary_brd = board; // mk a temp board
            temporary_brd.move_piece(from_square, to_square); // make that move
                // and move the rook too
            if ((board.get_piece(1ULL << from_square) == 'K' || board.get_piece(1ULL << from_square) == 'k') && abs(from_square - to_square) == 2) {
                if (to_square > from_square) temporary_brd.move_piece(from_square + 3, from_square + 1);
                else temporary_brd.move_piece(from_square - 4, from_square - 1);
            }
                //
            if(!is_in_check(temporary_brd, player_turn)) return true; // if that move does not leave you in check, wallahi you made it and there are legal moves
        }

        own_pieces &= own_pieces - 1;
    }

    return false;
}

// checks whether you are actively trying to attack your own piece, or moving smth that isn't yours
bool is_own_piece(uint64_t from_square, uint64_t to_square, int player_turn, const Board &board){
    uint64_t own_pieces = (player_turn == 1) ? board.get_white_pieces() : board.get_black_pieces(); // pickout your pieces depending on player turn
    if (!(from_square & own_pieces)) return false; // is that your piece?
    if (to_square & own_pieces) return false; // cannot capture your own piece
    return true;    
}
//

// checks whether that was a valid move according to that piece
bool Validator::is_valid_move(int from_index, int to_index, int player_turn, const Board &board){
    uint64_t from_mask = (1ULL << from_index);
    uint64_t to_mask = (1ULL << to_index);
    char piece_type = board.get_piece(from_mask);

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
            if (from_index == 4 && to_mask == (1ULL << 6)){
                return is_valid_castle(from_index, to_index, player_turn, board);
            }
            if (from_index == 4 && to_mask == (1ULL << 2)){
                return is_valid_castle(from_index, to_index, player_turn, board);
            }
            if (from_index == 60 && to_mask == (1ULL << 62)){
                return is_valid_castle(from_index, to_index, player_turn, board);
            }
            if (from_index == 60 && to_mask == (1ULL << 58)){
                return is_valid_castle(from_index, to_index, player_turn, board);
            }
            return is_valid_king_move(from_index, to_mask);

        default:
            return false;
    }
}
//

// main big boy function that posts to public
// and is calling the rest of all our functions
bool Validator::is_valid(int player_turn, int from_square, int to_square, const Board &board){
    uint64_t from_mask = (1ULL << from_square);
    uint64_t to_mask = (1ULL << to_square);

    // is that your piece?
    if(!is_own_piece(from_mask, to_mask, player_turn, board)){
        // cout << "You are either moving an enemy piece, or attacking your own!" << endl;
        return 0;
    }
    //

    // checks whether that was a valid move according to what piece it is
    if(!is_valid_move(from_square, to_square, player_turn, board)){
        // cout << "That is not a valid move! Try again" << endl;
        return 0;
    }
    //
    
    // is the king in check?
    Board temporary_board = board;
    temporary_board.move_piece(from_square, to_square);
        //  and move the rook too if its a castle
    if ((board.get_piece(from_mask) == 'K' || board.get_piece(from_mask) == 'k') &&abs(from_square - to_square) == 2) {
        if (to_square > from_square) temporary_board.move_piece(from_square + 3, from_square + 1);
        else temporary_board.move_piece(from_square - 4, from_square - 1);            
    }
        //
    if(is_in_check(temporary_board, player_turn)){
        return false;
    }
    //
        
    return 1;
}
//