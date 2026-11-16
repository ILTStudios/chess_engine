#include <iostream>
#include <limits>
#include "game.h"
#include "player.h"

using namespace std;

void display_menu(){
    cout << "\n========================\n";
    cout << "      CHESS ENGINE\n";
    cout << "========================\n";
    cout << "1. Play against another player\n";
    cout << "2. Play against computer\n";
    cout << "3. Load game\n";
    cout << "4. View board\n";
    cout << "5. Settings\n";
    cout << "0. Exit\n";
    cout << "========================\n";
    cout << "Select an option: ";
}

int main(){
    int menu_option;

    do
    {
        display_menu();
        cin >> menu_option;
        switch (menu_option)
        {
            case 1:{
                cout << "Starting Multiplayer...\n";
                Game game;
                game.initialise("std");
                game.run("", false);
                break;
            }
            case 2:{
                cout << "Playing against Computer... \n";
                Player player;
                Game game;
                game.initialise("std");
                game.run("", false);
                break;
            }
            case 3:{
                string FEN;
                cout << "Input a FEN: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, FEN);

                string moveset;
                cout << "Input a Moveset: ";
                getline(cin, moveset);

                if(moveset == "test"){
                    moveset = "e2e4 c7c5 g1f3 e7e6 d2d4 c5d4 f3d4 b8c6 d4b5 d7d6 c2c4 g8f6 b1c3 a7a6 b5a3 d6d5 c4d5 e6d5 e4d5 c6b4 f1e2 f8c5 e1g1 e8g8 e2f3 c8f5 c1g5 f8e8 d1d2 b7b5 a1d1 b4d3 a3b1 h7h6 g5h4 b5b4 c3a4 c5d6 h4g3 a8c8 b2b3 g7g5 g3d6 d8d6 g2g3 f6d7 f3g2 d6f6 a2a3 a6a5 a3b4 a5b4 d2a2 f5g6 d5d6 g5g4 a2d2 g8g7 f2f3 f6d6 f3g4 d6d4 g1h1 d7f6 f1f4 f6e4 d2d3 e4f2 f4f2 g6d3 f2d2 d4e3 d2d3 c8c1 a4b2 e3f2 b1d2 c1d1 b2d1 e8e1";
                }

                Game game;
                game.initialise(FEN);
                game.run(moveset, true);
                break;
            }
            case 0:{
                cout << "Exiting Program...\n";
                break;
            }
            default:
                cout << "Invalid Option!\n";
        }
    } while (menu_option != 0);

    return 0;
}