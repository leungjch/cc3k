// TODO: check max hp
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
    srand(1);

    // Set random seed (for actual demo)
    srand(time(nullptr));

    // Work with STL containers for command line arguments
    vector<string> args(argv, argv + argc);

    cout << "Welcome to CC3K!" << endl;

    // Game logic
    auto game = make_shared<CC3K>();

    // Text display
    auto textObserver = make_shared<TextObserver>(game.get(), 79, 25);

    game->attach(textObserver.get());

    shared_ptr<GraphicalObserver> graphicalObserver = nullptr;

    // Parse command line args
    for (int i = 0; i < args.size(); i++)
    {
        if (i == 0)
        {
            // Ignore the "./cc3k"
            continue;
        }
        if (args[i] == "-dlc")
        {
            graphicalObserver = make_shared<GraphicalObserver>(game.get(), 79, 25);
            game->attach(graphicalObserver.get());
        }
        // Otherwise it is a filename to a file containing five levels
        else
        {
            game->parseCustomLevels(args[i]);
        }
    }
    game->newGame();

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
            if (cmd == "no" || cmd == "so" || cmd == "ea" || cmd == "we" || cmd == "ne" || cmd == "nw" || cmd == "se" || cmd == "sw")
            {
                if (iss.eof())
                {
                    if (game->movePlayer(cmd) == -1) {
                        cout << "You died!\nTo play again, press r, to quit, press q."<< endl;
                        string choice;
                        if (cin >> choice) {
                            if (cmd == "r")
                            {
                                game->newGame();
                            }

                            else if (cmd == "q")
                            {
                                cout << "Exiting." << endl;
                                return 0;
                            }
                        }
                    }
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
                            if (game->movePlayer(cmd) == -1) {
                            cout << "You died!\nTo play again, press r, to quit, press q."<< endl;
                            string choice;
                            if (cin >> choice) {
                                if (cmd == "r")
                                {
                                    game->newGame();
                                    break;
                                }

                                else if (cmd == "q")
                                {
                                    cout << "Exiting." << endl;
                                    return 0;
                                }
                            }
                            }
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
                // TODO: check invalid dir
                iss >> dir;
                game->usePotion(dir);
            }

            // a direction: attacks the enemy in the specified direction, if the monster is in the immediately specified block (e.g. must
            // be one block north of the @).
            else if (cmd == "a")
            {
                string dir;
                // Read the direction
                // TODO: check invalid dir
                iss >> dir;
                game->playerAttack(dir);
            }

            // Specify starting race
            else if (cmd == "s")
            {
                game->setStartingRace(Player::RaceTypes::SHADE);
                game->addMessage("You will play as a Shade in the next life.", Color::CYAN);
            }
            else if (cmd == "d")
            {
                game->setStartingRace(Player::RaceTypes::DROW);
                game->addMessage("You will play as a Drow in the next life.", Color::CYAN);
            }
            else if (cmd == "v")
            {
                game->setStartingRace(Player::RaceTypes::VAMPIRE);
                game->addMessage("You will play as a Vampire in the next life.", Color::CYAN);
            }
            else if (cmd == "g")
            {
                game->setStartingRace(Player::RaceTypes::GOBLIN);
                game->addMessage("You will play as a Goblin in the next life.", Color::CYAN);
            }
            else if (cmd == "t")
            {
                game->setStartingRace(Player::RaceTypes::TROLL);
                game->addMessage("You will play as a Troll in the next life.", Color::CYAN);
            }

            // f: stops enemies from moving until this key is pressed again.
            else if (cmd == "f")
            {
                game->toggleStopEnemies();
            }

            // r: restarts the game-> All stats, inventory, and gold are reset. A new race should be selected.
            else if (cmd == "r")
            {
                game->newGame();
            }

            // q: allows the player to admit defeat and exit the game

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
