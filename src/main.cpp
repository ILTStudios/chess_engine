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
    int auto_play;
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