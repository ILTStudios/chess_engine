#include <iostream>
#include "game.h"
#include "player.h"

using namespace std;

int main(){
    int auto_play;
    cout << "Choose your play type (1/0):";
    cin >> auto_play;
    if (auto_play == 1)
    {
        cin.clear();
        cin.ignore(100, '\n');

        Game game;
        game.initialise("");
        cout << "We have initalised!" << endl;
        game.run("e2e4 e7e5 g1f3 b8c6 f1c4 f8c5 c2c3 g8f6 d2d3 d7d6 b1d2 c8e6", true);
    }

    else{
        Game game;
        game.initialise("");
        game.run("", false);
    }
}