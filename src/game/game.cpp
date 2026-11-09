#include <iostream>
#include "input.h" // gets and parses the input from the player
#include "board.h" // handles all the game board variables and what not
#include "validator.h" // contians all the code to validate moves

using namespace std;

int main(){
    int player_turn = 1;
    bool has_ended = false;

    int from_index;
    int to_index;

    class Board board;
    board.initialise();
    board.display();

    class Validator validator;
    validator.initialise();

    class Input input;

    while (has_ended == false)
    {
        // get input from player
        input.get_input(player_turn);
        from_index = input.get_input_from();
        to_index = input.get_input_to();

        // validate the move
        if(!validator.is_valid(player_turn, from_index, to_index, board)){
            cout << "Invalid Move! Try again\n";
            continue;
        }
        // change the board state
        board.move_piece(from_index, to_index);

        // display board
        board.display();

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
    }

    return 0;
}