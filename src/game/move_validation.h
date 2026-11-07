#pragma once
#include <cstdint>

class validator{
    public:
        

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
};