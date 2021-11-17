#include "cc3k.h"
#include "player/drow.h"
#include <iostream>
#include <string>
#include <memory>

using namespace std;

CC3K::CC3K() {}

void CC3K::init()
{
    // Get the map and read it into a floor
    theFloor.readMap("map.txt");

    // Generate the player and entities
    generatePlayer();
}

void CC3K::generatePlayer()
{
    thePlayer = make_shared<Drow>();

    // TODO: Random generation
    thePlayer->setX(10);
    thePlayer->setY(10);
}

void CC3K::movePlayer(string dir)
{
    if (dir == "no")
    {
        thePlayer->setY(thePlayer->getY()-1);
    }

    else if (dir == "so")
    {
        thePlayer->move(0,1);
    }

    else if (dir == "ea")
    {
        thePlayer->move(1,0);
    }

    else if (dir == "we")
    {
        thePlayer->move(-1,0);
    }

    else if (dir == "ne")
    {
        thePlayer->move(1,-1);
    }

    else if (dir == "se")
    {
        thePlayer->move(1,1);
    }

    else if (dir == "sw")
    {
        thePlayer->move(-1,1);
    }
}

void CC3K::display()
{
    /* Rendering algorithm:
     *   Copy over the environment to the display (which will overwrite the old display)
     *   Loop through all entities and overwrite the display with each entity's position
     *   Print out the new display
     */
    theDisplay = theFloor.getEnvironmentChar();

    // Place the player's position
    theDisplay[thePlayer->getY()][thePlayer->getX()] = thePlayer->getSymbol();

    // Loop through entities and overwrite display with each entity's position
    // ...

    // Print out the display
    for (int i = 0; i < theDisplay.size(); i++)
    {
        for (int j = 0; j < theDisplay[0].size(); j++)
        {
            cout << theDisplay[i][j];
        }
        cout << endl;
    }
}
