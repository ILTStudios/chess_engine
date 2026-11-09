#include <iostream>
#include "input.h"

using namespace std;

int Input::get_input_from(){
    return from_index;
}

int Input::get_input_to(){
    return to_index;
}

// converts a string input (e2) into a integer index (13)
int string_to_index(string square){
    int file = tolower(square[0]) - 'a';
    int rank = square[1] - '1';
    return (rank * 8) + file;
}

// checks whether that move is represented by out 8x8 board
bool on_board(string square){
    string from_square = square.substr(0, 2);
    string to_square = square.substr(2, 2);

    char from_file = tolower(from_square[0]);
    char from_rank = from_square[1];

    char to_file = tolower(to_square[0]);
    char to_rank = to_square[1];

    return  (from_file >= 'a' && from_file <= 'h') &&
            (to_file >= 'a' && to_file <= 'h') &&
            (from_rank >= '1' && from_rank <= '8') &&
            (to_rank >= '1' && to_rank <= '8');
}

int Input::authenticate_input(string move){
    // does this follow our convention for input?
    if(move.length() != 4){
        cout << "Please input from square to which square you would like to move (eg: e2e4)";
        return 0;
    }
    if(!on_board(move)){
        cout << "Please enter a file from a - h and a rank from 1 - 8";
        return 0;
    }
    //
    else{
        // everything seems alright, change the from and to masks
        string from_square = move.substr(0, 2);
        string to_square = move.substr(2, 2);

        from_index = string_to_index(from_square);
        to_index = string_to_index(to_square);
        //

        // cout << "From mask: " << from_mask << endl;
        // cout << "To mask: " << to_mask << endl;

        // cout << "From index: " << string_to_index(from_square) << endl;
        // cout << "To index: " << string_to_index(to_square) << endl;

        return 1; //change validate so that the next player has a go
    }
}

void Input::get_input(int player_turn){
    validate = 0;
    string raw_input;

    while (validate != 1)
    {
        cout << "Player " << ( (player_turn == 1) ? "White, ": "Black, ") << "Choose your move: ";
        cin >> raw_input;
        // is this a valid input?
        // is this on the board?
        validate += authenticate_input(raw_input);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    cout << endl;
}