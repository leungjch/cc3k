#include "cc3k.h"
#include "player/drow.h"
#include "potion/restoreHealth.h"
#include "potion/poisonHealth.h"
#include "potion/boostAtk.h"
#include "potion/woundAtk.h"
#include "potion/boostDef.h"
#include "potion/woundDef.h"

#include "potion/potion.h"
#include "stairway.h"
#include "cell.h"
#include "utils/color.h"


#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <memory>

using namespace std;

CC3K::CC3K() : levelNum{1}, playerGold{0} {}

void CC3K::init()
{
    /*
        20 enemies are spawned per floor (this number does not include dragons). Every
        chamber is equally likely to spawn any particular monster (similarly for floor
        tiles). We require that generation happens in the following order: player
        character location, stairway location, potions, gold, enemies. This is to allow
        us to more easily evaluate that your random generation is correctly implemented.
        Note that multiple objects (enemies, gold, and potions) cannot occupy the same
        cell on the game board. That is, no two objects can ever occupy the same space.
        The one exception to this is the case of gold. Typically, when a player
        character walks over gold, it is picked up. The exception to this is if the gold
        is associated with a still alive dragon; in this case, the player simply walks
        over the gold, without picking it. When the PC attempts to move on to a
        stairway, the next level is instead generated and displayed, with the PC
        spawning in a random position on the new level. Items and enemies should only
        ever spawn on a floor tile and never in a doorway, passage, or the stairs
        leading down to the next floor.
    */

    // Get the map and read it into a floor
    theFloor.readMap("map.txt");


    // Generate the player and entities
    generatePlayer();

    // Generate stairway
    generateStairway();

    // Generate potions
    generatePotions();


    // Generate gold

    // Generate enemies

}

// Helper function to compute new position after moving one unit in the specified direction
pair<int, int> getPosAtDirection(int x, int y, string dir)
{
    int dX = 0;
    int dY = 0;
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
    else {
        cerr << "Invalid direction" << endl;
    }
    return make_pair(x+dX, y+dY);

}

// Returns true if an entity is occupying a cell
bool CC3K::isOccupied(int x, int y) 
{
    // Check the player coordinates
    if (thePlayer->getX() == x && thePlayer->getY() == y)
    {
        return true;
    }

    // Check the staircase coordinates
    if (theStairway->getX() == x && theStairway->getY() == y)
    {
        return true;
    }

    // Check all potions
    for (auto potion : thePotions) 
    {
        if (potion->getX() == x && potion->getY() == y) 
        {
            return true;
        }
    }

    // TODO: Check gold coordinates

    // TODO: Check all enemy coordinates

    // Else no entity occupies this space, so return false
    return false;
}

// Generate a player in a random position in a chamber
void CC3K::generatePlayer()
{
    thePlayer = make_shared<Drow>();

    // Select a random chamber number
    int targetChamberNum = theFloor.getRandomChamberNum();

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
    cout << Color::GREEN << "Player character has spawned. " << Color::RESET << endl;

}

void CC3K::generateStairway()
{
    // Assumption: only the player has been generated at this point
    // Thus we only need to check collision with player
    theStairway = make_shared<Stairway>();

    int playerChamberNum = theFloor.chamberAt(thePlayer->getX(), thePlayer->getY());

    // Generate a random chamber number (to ensure each chamber has equal probability)
    // Ensure it is NOT EQUAL to player chamber
    int targetChamberNum = theFloor.getRandomChamberNum();;
    while (targetChamberNum == playerChamberNum)
    {
        targetChamberNum = theFloor.getRandomChamberNum();
    }
    while (true)
    {
        // Get random coordinates and a random chamber number
        int randX = theFloor.getRandomX();
        int randY = theFloor.getRandomY();
        int chamberNum = theFloor.chamberAt(randX, randY);

        // Check that chamber is not same as the chamber that player spawned in
        if (chamberNum == targetChamberNum && chamberNum != playerChamberNum)
        {
            theStairway->setX(randX);
            theStairway->setY(randY);
            break;
        }
    }
}

void CC3K::generatePotions() 
{
    thePotions.clear();
    for (int i = 0; i < NUM_POTIONS; i++)
    {
        bool isGenerating = true;

        // Generate a random potion type
        int potionType = rand() % Potion::NUM_POTION_TYPES + 1;

        auto newPotion = make_shared<Potion>();

        switch (potionType)
        {
            case 1:
            {
                newPotion = make_shared<RestoreHealth>();
                break;
            }
            case 2:
            {
                newPotion = make_shared<PoisonHealth>();
                break;

            }
            case 3:
            {
                newPotion = make_shared<BoostAtk>();
                break;

            }
            case 4:
            {
                newPotion = make_shared<WoundAtk>();
                break;
            }
            case 5:
            {
                newPotion = make_shared<BoostDef>();
                break;
            }
            case 6:
            {
                newPotion = make_shared<WoundDef>();
                break;

            }

        }


        // Generate a random chamber number (to ensure each chamber has equal probability)
        int targetChamberNum = theFloor.getRandomChamberNum();

        while (isGenerating)
        {
            // Get random coordinates and its associated chamber
            int randX = theFloor.getRandomX();
            int randY = theFloor.getRandomY();
            int chamberNum = theFloor.chamberAt(randX, randY);

            if (chamberNum == targetChamberNum && !isOccupied(randX, randY))
            {
                newPotion->setX(randX);
                newPotion->setY(randY);
                thePotions.push_back(newPotion);
                isGenerating = false;
            }
        }
    }
}

void CC3K::movePlayer(string dir)
{
    pair<int,int> newPos = getPosAtDirection(thePlayer->getX(), thePlayer->getY(), dir);
    // Compute the new position
    int newX = newPos.first;
    int newY = newPos.second;

    // Check if the new position is a tile
    // (i.e. don't move into a wall or empty space)
    if (isOccupied(newX, newY) || 
        (theFloor.cellAt(newX, newY).getChar() != Cell::TILE &&
        theFloor.cellAt(newX, newY).getChar() != Cell::DOOR &&
        theFloor.cellAt(newX, newY).getChar() != Cell::PASSAGE))
    {
        cout << Color::RED << "You cannot move there." << Color::RESET << endl;
        return;
    }

    // Check if the new position is a stairway
    // In this case, go to next level
    if (newX == theStairway->getX() && newY == theStairway->getY())
    {
        // Respawn everything
        init();
        // Increase level number
        levelNum += 1;

        cout << Color::BOLDYELLOW << "Now entering level " << levelNum << Color::RESET << endl;

        return;
    }

    // Else the new position is valid, move player there
    thePlayer->setX(newX);
    thePlayer->setY(newY);

    // Check collision against all entities
}

// Use a potion in the direction specified (from the player)
void CC3K::usePotion(string dir) 
{
    // Get the coordinates to check
    pair<int, int> checkPotionPos = getPosAtDirection(thePlayer->getX(), thePlayer->getY(), dir);

    // Check if there exists a potion in the specified direction
    int found = -1;
    for (int i = 0; i < thePotions.size(); i++) {
        if (thePotions[i]->getX() == checkPotionPos.first && thePotions[i]->getY() == checkPotionPos.second)
        {
            found = i;
            break;
        }
    }

    // If we did not find a potion at that location
    if (found == -1) 
    {
        cout << Color::RED << "There is no potion in that direction." << Color::RESET << endl;
        return;
    }
    // Potion is found, apply the potion and erase it from the potion vector
    else 
    {
        // Apply the effect to the player
        thePlayer->applyPotion(thePotions[found]);
        
        // Output message
        // E.g. "You used a Potion of Restore Health"
        cout << Color::BOLDMAGENTA << "You used a " << thePotions[found]->getName() << ". " << thePotions[found]->getDescription() << Color::RESET << endl;

        // Erase the potion from the vector
        thePotions.erase(thePotions.begin()+found);
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

    // Draw the staircase
    theDisplay[theStairway->getY()][theStairway->getX()] = theStairway->getSymbol();

    // Draw the potions
    for (auto potion : thePotions)
    {
        theDisplay[potion->getY()][potion->getX()] = potion->getSymbol();
    }

    // TODO: Draw the gold piles

    // TODO: Draw enemies

    // Print out the display
    for (int i = 0; i < theDisplay.size(); i++)
    {
        for (int j = 0; j < theDisplay[0].size(); j++)
        {
            cout << theDisplay[i][j];
        }
        cout << endl;
    }

    // Print out race
    cout << "Race: " << thePlayer->getName();

    // Print out player gold
    cout << " Gold: " << playerGold << endl;

    // Print out HP
    cout << "HP: " << thePlayer->getHP() << endl;

    // Print out Atk
    cout << "Atk: " << thePlayer->getAtk() << endl;

    // Print out Def
    cout << "Def: " << thePlayer->getDef() << endl;

    // Print out action (for any actions) that occur
    cout << "Action: " << endl;

}
