#include <iostream>
#include "game.h"
#include "player.h"

using namespace std;

int main(){
    int auto_play;
    cout << "Would you like to play move by move (0) or input a moveset string (1) eg: 'e2e4 e7e5': ";
    cin >> auto_play;
    if (auto_play == 1)
    {
        cin.clear();
        cin.ignore(100, '\n');
        string moveset;
        cout << "Choose the moveset you would like to play through: ";
        getline(cin, moveset);

        Game game;
        game.initialise("");
        cout << "We have initalised!" << endl;
        game.run(moveset, true);
    }

    else{
        Game game;
        game.initialise("");
        game.run("", false);
    }
}