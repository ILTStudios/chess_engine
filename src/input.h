#include <iostream>

#pragma once
#include <cstdint>

using namespace std;

class Input{
    public:
        int get_input_from();
        int get_input_to();
        string get_promotion_piece();
        void get_input(int player_turn); // gets the players input
        int authenticate_input(string move); // is that a valid entry?
    private:
        int validate;
        int from_index;
        int to_index;
        string promotion_piece = "\0";
};