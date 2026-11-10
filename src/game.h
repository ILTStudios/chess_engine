#pragma once
using namespace std;

class Game{
    public:
        // functions
        void initialise(); // initialises the game
        void get_input(); // redundant, don't really need it for browser
        void make_move(int from, int to); // redundant too
        string validate(string move);
        void update_game_state(int from, int to, int pt);
        //
    private:
        // variables
        int player_turn; bool has_ended; int from_index; int to_index;
        //

        // internal state
        class Board board;
        class Validator validator;
        class Input input;
        //
};