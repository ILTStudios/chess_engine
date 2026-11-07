#pragma once
#include <cstdint>

class Board;

class Validator{
    public:
        bool is_valid(int player_turn, int from_square, int to_square, const Board & board); // returns 0 if not valid move, returns 1 if vaild
        void initialise(); // initialises all validation data

    private:
        // general piece move validation biboards
        uint64_t knight_moves[64];
        uint64_t w_pawn_moves[64];
        uint64_t w_pawn_captures[64];
        uint64_t b_pawn_moves[64];
        uint64_t b_pawn_captures[64];
        uint64_t king_moves[64];
        uint64_t rook_moves[64];
        uint64_t bishop_moves[64];
        uint64_t queen_moves[64];
        //

        // special piece move validation data
        int enpassen_square;
        bool white_castle_ks;
        bool white_castle_qs;
        bool black_castle_ks;
        bool black_castle_qs;
        //

        // move bitboard functions
        void init_knight_moves();
        void init_pawn_moves();
        void init_king_moves();
        void init_rook_moves();
        void init_bishop_moves();
        //

        // validation functions
        bool is_valid_move(uint64_t from_mask, uint64_t to_mask, int player_turn, const Board &board);
        bool is_valid_pawn_move(int from_mask, uint64_t to_mask, int player_turn, const Board &board);
        bool is_valid_knight_move(int from_mask, uint64_t to_mask);
        bool is_valid_bishop_move(int from_mask, uint64_t to_mask, const Board &board);
        bool is_valid_rook_move(int from_mask, uint64_t to_mask, const Board &board);
        bool is_valid_king_move(int from_mask, uint64_t to_mask);
        bool is_valid_queen_move(int from_mask, uint64_t to_mask, const Board &board);
};