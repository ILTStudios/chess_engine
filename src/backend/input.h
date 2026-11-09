#include <iostream>

#pragma once
#include <cstdint>

using namespace std;

class Input{
    public:
        int get_input_from();
        int get_input_to();
        void get_input(int player_turn); // gets the players input

    private:
        int validate;
        int from_index;
        int to_index;
        int authenticate_input(string move); // is that a valid entry?
};