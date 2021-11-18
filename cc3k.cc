#include "cc3k.h"
#include "player/drow.h"
#include "cell.h"
#include "utils/color.h"
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
    thePlayer->setY(5);

}

void CC3K::movePlayer(string dir)
{
    // Get the current player coordinates

    int dY = 0;
    int dX = 0;
    if (dir == "no")
    {
        dY = -1;
    }

    else if (dir == "so")
    {
        dY = 1;
    }

    else if (dir == "ea")
    {
        dX = 1;
    }

    else if (dir == "we")
    {
        dX = -1;
    }

    else if (dir == "ne")
    {
        dY = -1; 
        dX = 1;
    }

    else if (dir == "se")
    {
        dY = 1;
        dX = 1;
    }

    else if (dir == "sw")
    {
        dY = 1;
        dX = -1;
    }

    // Compute the new position
    int newY = thePlayer->getY() + dY;
    int newX = thePlayer->getX() + dX;

    // Check if the new position is a tile
    // (i.e. don't move into a wall or empty space)
    if (theFloor.cellAt(newX, newY).getChar() != Cell::TILE &&
        theFloor.cellAt(newX, newY).getChar() != Cell::DOOR &&
        theFloor.cellAt(newX, newY).getChar() != Cell::PASSAGE
    )
    {
        cout << Color::RED << "You cannot move there." << Color::RESET << endl;
        return;
    }

    // Else the new position is valid, move player there
    thePlayer->setX(newX);
    thePlayer->setY(newY);

    // Check collision against all entities

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
