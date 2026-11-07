#include <iostream>
#include "validator.h"

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
        
        if(square >= 8 && square < 15){
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

    enpassen_square = NULL;
    white_castle_ks = true;
    white_castle_qs = true;
    black_castle_ks = true;
    black_castle_qs = true;

    cout << endl;
}

bool Validator::is_valid(){

    return 1;
}