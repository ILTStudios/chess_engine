#include <iostream>
#include "input.h" // gets and parses the input from the player
#include "board.h" // handles all the game board variables and what not
#include "validator.h" // contians all the code to validate moves

using namespace std;

int main(){
    int player_turn = 1;
    bool has_ended = false;

    int raw_from_index;
    int raw_to_index;

    class Board board;
    board.initialise();

    class Validator validator;
    validator.initialise();

    class Input input;

    while (has_ended == false)
    {
        board.display();

        // get input from player
        input.get_input(player_turn);
        raw_from_index = input.get_input_from();
        raw_to_index = input.get_input_to();

        // validate the move

        // change player turn
        // player_turn = 0;
        // input.get_input(player_turn);
        // raw_from_index = input.get_input_from();
        // raw_to_index = input.get_input_to();

        // validate the move

        // change the board state
        board.move_piece(raw_from_index, raw_to_index);

        //reset player turn
        player_turn = 1;
    }

    return 0;
}