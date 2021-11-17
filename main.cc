#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "utils/color.h"
#include "cc3k.h"

using namespace std;

int main(int argc, char *argv[]) 
{
    // Work with STL containers for command line arguments
    vector<string> args(argv, argv+argc);

    cout << "Welcome to CC3K!" << endl;

    CC3K game;

    game.init();

    string cmd;


    game.display();

    while (cin >> cmd)
    {
        if (cmd == "no" || cmd == "so" || cmd == "ea" || cmd == "we" || cmd == "ne" || cmd == "se" || cmd == "sw")
        {
            game.movePlayer(cmd);
        }




        // u direction: uses the potion indicated by the direction (e.g. no, so, ea).
        else if (cmd == "u")
        {

        }

        // a direction: attacks the enemy in the specified direction, if the monster is in the immediately specified block (e.g. must
        // be one block north of the @).
        else if (cmd == "a")
        {

        }

        // Specify race
        else if (cmd == "s")
        {

        }
        else if (cmd == "d")
        {

        }
        else if (cmd == "v")
        {
            
        }
        else if (cmd == "g")
        {
            
        }
        else if (cmd == "t")
        {
            
        }

        // f: stops enemies from moving until this key is pressed again.
        else if (cmd == "f")
        {

        }

        // r: restarts the game. All stats, inventory, and gold are reset. A new race should be selected.
        else if (cmd == "r")
        {

        }

        // q: allows the player to admit defeat and exit the game.
        else if (cmd == "q")
        {
            cout << "Exiting." << endl;
            break;
        }

        // Render the display
        game.display();
    }
}
