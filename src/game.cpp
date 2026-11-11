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
    board.display();
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
            input.authenticate_input(move);
            from_index = input.get_input_from();
            to_index = input.get_input_to();

            make_move();
            this_thread::sleep_for(chrono::milliseconds(100));
        }
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