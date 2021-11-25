#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <cstdlib>
#include <sstream>
#include "utils/color.h"
#include "player/player.h"
#include "display/textObserver.h"
#include "display/graphicalObserver.h"

#include "cc3k.h"

using namespace std;

int main(int argc, char *argv[])
{

    // Set seed
    // Deterministic seed for testing
    // srand(42);

    // Set random seed (for actual demo)
    srand(time(nullptr));

    // Work with STL containers for command line arguments
    vector<string> args(argv, argv + argc);

    cout << "Welcome to CC3K!" << endl;

    // Game logic
    auto game = make_shared<CC3K>();

    // Text display
    auto textObserver = make_shared<TextObserver>(game, 79, 25);
    auto graphicalObserver = make_shared<GraphicalObserver>(game, 79, 25);

    game->attach(textObserver);
    game->attach(graphicalObserver);
    game->init();

    string cmdLine;

    // Create the display

    game->render();

    while (getline(cin, cmdLine))
    {
        istringstream iss(cmdLine);
        string cmd;
        while (iss >> cmd)
        {

            // Player movement
            if (cmd == "no" || cmd == "so" || cmd == "ea" || cmd == "we" || cmd == "ne" || cmd == "se" || cmd == "sw")
            {
                if (iss.eof())
                {
                    game->movePlayer(cmd);
                }
                // BONUS FEATURE: specify a number to repeatedly move
                else
                {
                    string numRepeat;
                    iss >> numRepeat;
                    try
                    {
                        for (int i = 0; i < stoi(numRepeat); i++)
                        {
                            game->movePlayer(cmd);
                        }
                    }
                    // Exception handling (if numRepeat cannot cast to an int)
                    catch (const std::invalid_argument &e)
                    {
                        game->movePlayer(cmd);
                    }
                }
            }

            // u direction: uses the potion indicated by the direction (e.g. no, so, ea).
            else if (cmd == "u")
            {
                string dir;
                // Read the direction
                // TODO: error checking
                iss >> dir;
                game->usePotion(dir);
            }

            // a direction: attacks the enemy in the specified direction, if the monster is in the immediately specified block (e.g. must
            // be one block north of the @).
            else if (cmd == "a")
            {
            }

            // Specify starting race
            else if (cmd == "s")
            {
                game->setStartingRace(Player::RaceTypes::SHADE);
                cout << Color::CYAN << "You will play as a Shade in the next life." << Color::RESET << endl;
            }
            else if (cmd == "d")
            {
                game->setStartingRace(Player::RaceTypes::DROW);
                cout << Color::CYAN << "You will play as a Drow in the next life." << Color::RESET << endl;
            }
            else if (cmd == "v")
            {
                game->setStartingRace(Player::RaceTypes::VAMPIRE);
                cout << Color::CYAN << "You will play as a Vampire in the next life." << Color::RESET << endl;
            }
            else if (cmd == "g")
            {
                game->setStartingRace(Player::RaceTypes::GOBLIN);
                cout << Color::CYAN << "You will play as a Goblin in the next life." << Color::RESET << endl;
            }
            else if (cmd == "t")
            {
                game->setStartingRace(Player::RaceTypes::TROLL);
                cout << Color::CYAN << "You will play as a Troll in the next life." << Color::RESET << endl;
            }

            // f: stops enemies from moving until this key is pressed again.
            else if (cmd == "f")
            {
            }

            // r: restarts the game-> All stats, inventory, and gold are reset. A new race should be selected.
            else if (cmd == "r")
            {
                game->init();
            }

            // q: allows the player to admit defeat and exit the game->
            else if (cmd == "q")
            {
                cout << "Exiting." << endl;
                return 0;
            }

            // Render the display after every command
            game->render();

        }
    }
}
