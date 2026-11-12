#pragma once
#include <cstdint>
using namespace std;

class Board{
    public:
        void initialise(string FEN);
        void display();
        void remove_piece(uint64_t mask);        
        void move_piece(int from_square, int to_square);
        void promote_piece(int square, char promotion);

        // public getter functions to expose board data
            // white
        uint64_t get_white_pawns() const;
        uint64_t get_white_rooks() const;
        uint64_t get_white_knights() const;
        uint64_t get_white_bishops() const;
        uint64_t get_white_king() const;
        uint64_t get_white_queen() const;
        uint64_t get_white_pieces() const;
            //
            
            // black
        uint64_t get_black_pawns() const;
        uint64_t get_black_rooks() const;
        uint64_t get_black_knights() const;
        uint64_t get_black_bishops() const;
        uint64_t get_black_king() const;
        uint64_t get_black_queen() const;
        uint64_t get_black_pieces() const;
            //
        //

    private:
        // all the white piece location bitboards
        uint64_t white_pawns;
        uint64_t white_rooks;
        uint64_t white_knights;
        uint64_t white_bishops;
        uint64_t white_king;
        uint64_t white_queen;
        // 

        // all the black piece location bitboards
        uint64_t black_pawns;
        uint64_t black_rooks;
        uint64_t black_knights;
        uint64_t black_bishops;
        uint64_t black_king;
        uint64_t black_queen;
        //
        
        // supplementary bitboards
        uint64_t white_pieces;
        uint64_t black_pieces;
        uint64_t occupied_spaces;
        uint64_t empty_squares;
        //
};