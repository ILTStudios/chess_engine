#include <iostream>
#include <cstdint>

#include "board.h"

using namespace std;

// initialises the board variables
void Board::initialise(){
    // setting up all piece bitboards for standard game (white)
    white_rooks = (1ULL << 0) | (1ULL << 7);
    white_knights = (1ULL << 1) | (1ULL << 6);
    white_bishops = (1ULL << 2) | (1ULL << 5);
    white_queen = (1ULL << 3);
    white_king = (1ULL << 4);
    white_pawns = (0xFFULL << 8);
    //

    // setting up all piece bitboards for standard game (black)
    black_rooks = (1ULL << 63) | (1ULL << 56);
    black_knights = (1ULL << 62) | (1ULL << 57);
    black_bishops = (1ULL << 61) | (1ULL << 58);
    black_queen = (1ULL << 59);
    black_king = (1ULL << 60);
    black_pawns = (0xFFULL << 48);
    //

    // setting up all supplementary bitboards for standard game
    white_pieces = white_rooks | white_knights | white_bishops | white_queen | white_king | white_pawns;
    black_pieces = black_rooks | black_knights | black_bishops | black_queen | black_king | black_pawns;
    occupied_spaces = white_pieces | black_pieces;
    empty_squares = ~occupied_spaces;
    //
}

// displays the board
void Board::display(){
    // we count backwards from black to white, so from rank 8 to 1
    for(int rank = 7; rank >= 0; rank -= 1){
        cout << (rank + 1) << " "; // display the rank we are on

        // loop through all 8 files on that rank and output what should come
        // depending on what bitboard * mask returns true

        for(int file = 0; file < 8; file += 1){
            int square = (rank * 8) + file;
            uint64_t mask = (1ULL << square);

            char symbol = '.';
            if      (white_pawns & mask)   symbol = 'P';
            else if (white_knights & mask) symbol = 'N';
            else if (white_bishops & mask) symbol = 'B';
            else if (white_rooks & mask)   symbol = 'R';
            else if (white_queen & mask)   symbol = 'Q';
            else if (white_king & mask)    symbol = 'K';
            
            else if (black_pawns & mask)   symbol = 'p';
            else if (black_knights & mask) symbol = 'n';
            else if (black_bishops & mask) symbol = 'b';
            else if (black_rooks & mask)   symbol = 'r';
            else if (black_queen & mask)   symbol = 'q';
            else if (black_king & mask)    symbol = 'k';

            cout << symbol << " ";
        }
        cout << endl;
    }
    cout << "  A B C D E F G H\n\n";
}

// removes whatever piece and mask align
void Board::remove_piece(uint64_t mask){
    white_pawns   &= ~mask;
    white_knights &= ~mask;
    white_bishops &= ~mask;
    white_rooks   &= ~mask;
    white_queen   &= ~mask;
    white_king    &= ~mask;

    black_pawns   &= ~mask;
    black_knights &= ~mask;
    black_bishops &= ~mask;
    black_rooks   &= ~mask;
    black_queen   &= ~mask;
    black_king    &= ~mask;
}

// moves a piece and updates the board state
void Board::move_piece(int from_square, int to_square){
    uint64_t from_mask = (1ULL << from_square);
    uint64_t to_mask = (1ULL << to_square);

    // check if its a capture
    if(occupied_spaces & to_mask){
        remove_piece(to_mask);
    }

    // check through if loops which piece the from_mask relates to
    // then change that piece to hold that from_mask
    if      (white_pawns & from_mask)   { white_pawns   = (white_pawns & ~from_mask) | to_mask; }
    else if (white_knights & from_mask) { white_knights = (white_knights & ~from_mask) | to_mask; }
    else if (white_bishops & from_mask) { white_bishops = (white_bishops & ~from_mask) | to_mask; }
    else if (white_rooks & from_mask)   { white_rooks   = (white_rooks & ~from_mask) | to_mask; }
    else if (white_queen & from_mask)   { white_queen   = (white_queen & ~from_mask) | to_mask; }
    else if (white_king & from_mask)    { white_king    = (white_king & ~from_mask) | to_mask; }
    
    else if (black_pawns & from_mask)   { black_pawns   = (black_pawns & ~from_mask) | to_mask; }
    else if (black_knights & from_mask) { black_knights = (black_knights & ~from_mask) | to_mask; }
    else if (black_bishops & from_mask) { black_bishops = (black_bishops & ~from_mask) | to_mask; }
    else if (black_rooks & from_mask)   { black_rooks   = (black_rooks & ~from_mask) | to_mask; }
    else if (black_queen & from_mask)   { black_queen   = (black_queen & ~from_mask) | to_mask; }
    else if (black_king & from_mask)    { black_king    = (black_king & ~from_mask) | to_mask; }
    //

    // update all supplementary functions now
    white_pieces = white_rooks | white_knights | white_bishops | white_queen | white_king | white_pawns;
    black_pieces = black_rooks | black_knights | black_bishops | black_queen | black_king | black_pawns;
    occupied_spaces = white_pieces | black_pieces;
    empty_squares = ~occupied_spaces;
    //
}

// getter functions for all board private variables
    // white
uint64_t Board::get_white_pawns() const{
    return white_pawns;
}
uint64_t Board::get_white_rooks() const{
    return white_rooks;
}
uint64_t Board::get_white_knights() const{
    return white_knights;
}
uint64_t Board::get_white_bishops() const{
    return white_bishops;
}
uint64_t Board::get_white_king() const{
    return white_king;
}
uint64_t Board::get_white_queen() const{
    return white_queen;
}
uint64_t Board::get_white_pieces() const{
    return white_pieces;
}
    //
    
    // black
uint64_t Board::get_black_pawns() const{
    return black_pawns;
}
uint64_t Board::get_black_rooks() const{
    return black_rooks;
}
uint64_t Board::get_black_knights() const{
    return black_knights;
}
uint64_t Board::get_black_bishops() const{
    return black_bishops;
}
uint64_t Board::get_black_king() const{
    return black_king;
}
uint64_t Board::get_black_queen() const{
    return black_queen;
}
uint64_t Board::get_black_pieces() const{
    return black_pieces;
}
    //