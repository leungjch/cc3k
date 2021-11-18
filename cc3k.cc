#include "cc3k.h"
#include "player/drow.h"
#include "cell.h"
#include "utils/color.h"
#include <iostream>
#include <string>
#include <memory>

using namespace std;

CC3K::CC3K() : levelNum{1} {}

void CC3K::init()
{
    /*
        We require that generation happens in the following order: player character location, stairway location, potions, gold,
        enemies. This is to allow us to more easily evaluate that your random generation is correctly implemented.
        Note that multiple objects (enemies, gold, and potions) cannot occupy the same cell on the game board. That is, no two
        objects can ever occupy the same space. The one exception to this is the case of gold. Typically, when a player character
        walks over gold, it is picked up. The exception to this is if the gold is associated with a still alive dragon; in this case, the
        player simply walks over the gold, without picking it.
        When the PC attempts to move on to a stairway, the next level is instead generated and displayed, with the PC spawning
        in a random position on the new level.
        Items and enemies should only ever spawn on a floor tile and never in a doorway, passage, or the stairs leading down to
        the next floor.
    */

    // Get the map and read it into a floor
    theFloor.readMap("map.txt");

    int playerChamberNum = theFloor.getRandomChamberNum();

    // Generate the player and entities
    generatePlayer(playerChamberNum);

    // Generate stairway
    generateStairway(playerChamberNum);

    // Generate potions

    // Generate gold

    // Generate enemies

}

// Generate a player in a random position in a chamber
void CC3K::generatePlayer(int targetChamberNum)
{
    thePlayer = make_shared<Drow>();

    while (true)
    {
        // Get random coordinates
        int randX = theFloor.getRandomX();
        int randY = theFloor.getRandomY();
        int chamberNum = theFloor.chamberAt(randX, randY);

        // Check the chamber
        if (chamberNum == targetChamberNum)
        {
            thePlayer->setX(randX);
            thePlayer->setY(randY);
            break;
        }

    }

}

void CC3K::generateStairway(int playerChamberNum)
{
    // Assumption: only the player has been generated at this point
    // Thus we only need to check collision with player

    while (true)
    {
        // Get random coordinates and a random chamber number
        int randX = theFloor.getRandomX();
        int randY = theFloor.getRandomY();
        int chamberNum = theFloor.chamberAt(randX, randY);

        // Check that chamber is not same as the chamber that player spawned in
        if (chamberNum != -1 && chamberNum != playerChamberNum)
        {
            theStairway.setX(randX);
            theStairway.setY(randY);
            break;
        }
    }
}

void CC3K::movePlayer(string dir)
{


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
        theFloor.cellAt(newX, newY).getChar() != Cell::PASSAGE)
    {
        cout << Color::RED << "You cannot move there." << Color::RESET << endl;
        return;
    }

    // Check if the new position is a stairway
    // In this case, go to next level
    if (newX == theStairway.getX() && newY == theStairway.getY())
    {
        // Respawn everything
        init();
        // Increase level number
        levelNum += 1;

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

    // Draw the staircase
    theDisplay[theStairway.getY()][theStairway.getX()] = theStairway.getSymbol();


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
