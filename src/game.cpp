#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include "input.h" // gets and parses the input from the player
#include "board.h" // handles all the game board variables and what not
#include "validator.h" // contians all the code to validate moves
#include "game.h" // includes the game header

using namespace std;

void Game::initialise(string FEN){
    player_turn = 1;
    has_ended = false;

    // initialise and display the board
    board.initialise(FEN);
    //

    // initialise the validator
    validator.initialise();
    //
}

void Game::run(string MOVESET, bool automatic){
    if(automatic == false){
        while(has_ended == false){
        input.get_input(player_turn);
        from_index = input.get_input_from();
        to_index = input.get_input_to();

        make_move();
        }
    }
    else{
        vector<string> moves;
        stringstream ss(MOVESET);
        string move;

        while (ss >> move) {
            moves.push_back(move);
        }

        for (string move : moves){
            if (has_ended) break;
            
            input.authenticate_input(move);
            from_index = input.get_input_from();
            to_index = input.get_input_to();

            make_move();
            board.display();
            this_thread::sleep_for(chrono::milliseconds(50));
        }
    }
}

void Game::make_move(){
    // validate the move
    if(!validator.is_valid(player_turn, from_index, to_index, board)){
        cout << "Invalid Move! Try again\n";
        return;
    }
    //

    char piece = validator.what_piece((1ULL << from_index), board);

    // check if en_passen
    bool is_en_passant = (piece == 'P' || piece == 'p') && to_index == validator.enpassen_square && (from_index % 8 != to_index % 8);
    if(is_en_passant){
        int direction = (player_turn == 1) ? 8 : -8;
        int captured_square = to_index - direction;

        board.remove_piece(captured_square);
    }
    // check if casteling and then move rook pls
    if (piece == 'K' || piece == 'k') {
        if (abs(from_index - to_index) == 2) {
            // ks
            if (to_index > from_index) board.move_piece(from_index + 3, from_index + 1);
            // qs
            else board.move_piece(from_index - 4, from_index - 1);
        }
    }
    
    // change the board state
    board.move_piece(from_index, to_index);

        // update all the special move cases
    validator.enpassen_square = -1;
    if ((piece == 'P' || piece == 'p') && abs(from_index - to_index) == 16) validator.enpassen_square = (from_index + to_index) / 2;

    string promotion_piece = input.get_promotion_piece();
    if(!promotion_piece.empty()){
        if (piece == 'P' && to_index >= 56) {
            board.promote_piece(to_index, promotion_piece[0]);
        }
        else if (piece == 'p' && to_index <= 7) {
            board.promote_piece(to_index, promotion_piece[0]);
        }
    }
    
    validator.update_castleing_rights(from_index, to_index);
    //

    // display board
    board.display();
    //

    // switch player
    player_turn = (player_turn == 1) ? 0 : 1;
    //

    // check whether you are in stalemate or checkmate
    if(validator.is_in_checkmate(board, player_turn) == true){
        has_ended = true;
        cout << "The game has ended in checkmate!" << endl;
    }
    if(validator.is_in_stalemate(board, player_turn) == true){
        has_ended = true;
        cout << "The game has ended in stalemate!" << endl; 
    }
    //
}