#pragma once
using namespace std;

#include "board.h"
#include "validator.h"
#include "input.h"

#include <vector>
#include <string>
#include <thread>
#include <chrono>


class Game{
    public:
        // functions
        void initialise(string FEN); // initialises the game
        void get_input(); // redundant, don't really need it for browser
        void make_move(); // redundant too
        void run(string MOVESET, bool automatic); // main entrance to run the chess board
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
