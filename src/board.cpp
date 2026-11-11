#include <iostream>
#include <sstream>
#include <cstdint>

#include "board.h"

using namespace std;

// initialises the board variables
void Board::initialise(string FEN){
    // reset all pieces
    white_rooks = white_knights = white_bishops = white_queen = white_king = white_pawns = 0ULL;
    black_rooks = black_knights = black_bishops = black_queen = black_king = black_pawns = 0ULL;
    //

    if (FEN.empty()) {
        FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    }

    string placement;
    stringstream ss(FEN);
    ss >> placement;

    int rank = 0;
    int file = 0;

    for(char c : placement){

        if(c == '/'){
            rank += 1;
            file = 0;
            continue;
        }

        if (isdigit(static_cast<unsigned char>(c))) {
            int number_of_empty = c - '0';
            file += number_of_empty;
            continue;
        }

        int square = (7 - rank) * 8 + file;
        uint64_t square_mask = 1ULL << square;

        switch (c) {

            // White pieces
            case 'R': white_rooks   |= square_mask; break;
            case 'N': white_knights |= square_mask; break;
            case 'B': white_bishops |= square_mask; break;
            case 'Q': white_queen   |= square_mask; break;
            case 'K': white_king    |= square_mask; break;
            case 'P': white_pawns   |= square_mask; break;

            // Black pieces
            case 'r': black_rooks   |= square_mask; break;
            case 'n': black_knights |= square_mask; break;
            case 'b': black_bishops |= square_mask; break;
            case 'q': black_queen   |= square_mask; break;
            case 'k': black_king    |= square_mask; break;
            case 'p': black_pawns   |= square_mask; break;
        }

        file += 1;

    }

    white_pieces =
        white_rooks | white_knights | white_bishops |
        white_queen | white_king | white_pawns;

    black_pieces =
        black_rooks | black_knights | black_bishops |
        black_queen | black_king | black_pawns;

    occupied_spaces = white_pieces | black_pieces;
    empty_squares = ~occupied_spaces;

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