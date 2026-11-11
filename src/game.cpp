#include <iostream>
#include "input.h" // gets and parses the input from the player
#include "board.h" // handles all the game board variables and what not
#include "validator.h" // contians all the code to validate moves
#include "game.h" // includes the game header

using namespace std;

void Game::initialise(){
    player_turn = 1;
    has_ended = false;

    // initialise and display the board
    board.initialise("");
    board.display();
    //

    // initialise the validator
    validator.initialise();
    //
}

void Game::run(){
    while(has_ended == false){
        input.get_input(player_turn);
        from_index = input.get_input_from();
        to_index = input.get_input_to();

        make_move();
    }
}

string Game::validate(){
    if(!validator.is_valid(player_turn, from_index, to_index, board)) return "Invalid! Try again";
    if(validator.is_in_checkmate(board, player_turn)) return "Checkmate! Game end";
    if(validator.is_in_stalemate(board, player_turn)) return "Stalemate! Game end";

    return "N/A";
}

void Game::update_game_state(int from, int to, int pt){
    board.move_piece(from, to);
    player_turn = pt;
}

void Game::make_move(){
    // validate the move
    if(!validator.is_valid(player_turn, from_index, to_index, board)){
        cout << "Invalid Move! Try again\n";
        return;
    }
    //

    // change the board state
    board.move_piece(from_index, to_index);
    //

    // display board
    board.display();
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

    // switch player
    player_turn = (player_turn == 1) ? 0 : 1;
    //
}