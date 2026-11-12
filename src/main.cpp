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
        cin.clear(); cin.ignore(100, '\n');
        string FEN;
        cout << "What FEN would you like to use? (std for normal start): ";
        cin >> FEN;

        Game game;
        game.initialise(FEN);
        cout << "We have initalised!" << endl;
        game.run(moveset, true);
    }

    else{
        Game game;
        cin.clear(); cin.ignore(100, '\n');
        string FEN;
        cout << "What FEN would you like to use? (std for normal start): ";
        cin >> FEN;
        game.initialise(FEN);
        game.run("", false);
    }
}