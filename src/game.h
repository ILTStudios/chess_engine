#pragma once
using namespace std;

#include "board.h"
#include "validator.h"
#include "input.h"


class Game{
    public:
        // functions
        void initialise(); // initialises the game
        void get_input(); // redundant, don't really need it for browser
        void make_move(); // redundant too
        string validate();
        void update_game_state(int from, int to, int pt);
        void run();
        //
    private:
        // variables
        int player_turn; bool has_ended; int from_index; int to_index;
        //

        // internal state
        Board board;
        Validator validator;
        Input input;
        //
};