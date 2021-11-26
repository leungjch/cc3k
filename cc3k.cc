#include "cc3k.h"
#include "player/player.h"
#include "player/drow.h"
#include "player/shade.h"
#include "player/goblin.h"
#include "player/troll.h"
#include "player/vampire.h"

#include "potion/restoreHealth.h"
#include "potion/poisonHealth.h"
#include "potion/boostAtk.h"
#include "potion/woundAtk.h"
#include "potion/boostDef.h"
#include "potion/woundDef.h"

#include "treasure/gold.h"
#include "treasure/smallPile.h"
#include "treasure/normalPile.h"
#include "treasure/dragonHoard.h"
#include "treasure/merchantHoard.h"

#include "potion/potion.h"
#include "stairway.h"
#include "floor.h"

#include "cell.h"
#include "utils/color.h"

#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <memory>

using namespace std;

CC3K::CC3K() : levelNum{1}, playerGold{0}, startingRace{Player::RaceTypes::SHADE}, 
    theFloor{make_shared<Floor>()} 
    {}

CC3K::~CC3K()
{
    for (int i = 0; i < observers.size(); i++)
    {
        detach(observers[i]);
    }
    observers.clear();

}



void CC3K::newGame()
{
    levelNum = 1;
    // Generate the player
    generatePlayer();

    // Generate the level
    newLevel();
}

void CC3K::newLevel()
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
    theFloor->readMap("map.txt");

    // If we're not on the first level, the player may have some permanent potions
    // So we should apply them and discard the effects of the temporary potions
    thePlayer->applyPermanentPotions();


    // Place the player at a random chamber
    placePlayer();


    // Generate stairway
    generateStairway();

    // Generate potions
    generatePotions();

    // Generate gold
    generateGold();

    // Generate enemies

}

void CC3K::placePlayer()
{
    // Select a random chamber number
    int targetChamberNum = theFloor->getRandomChamberNum();

    while (true)
    {
        // Get random coordinates
        int randX = theFloor->getRandomX();
        int randY = theFloor->getRandomY();
        int chamberNum = theFloor->chamberAt(randX, randY);

        // Check the chamber
        if (chamberNum == targetChamberNum)
        {
            thePlayer->setX(randX);
            thePlayer->setY(randY);
            break;
        }
    }
    messages.emplace_back("Player character has spawned. ", Color::GREEN);

}

// Helper function to compute new position after moving one unit in the specified direction
pair<pair<int, int>, string> getPosAtDirection(int x, int y, string dir)
{
    int dX = 0;
    int dY = 0;
    string direction;
    if (dir == "no")
    {
        dY = -1;
        direction = "North";
    }

    else if (dir == "so")
    {
        dY = 1;
        direction = "South";
    }

    else if (dir == "ea")
    {
        dX = 1;
        direction = "East";
    }

    else if (dir == "we")
    {
        dX = -1;
        direction = "West";
    }

    else if (dir == "ne")
    {
        dY = -1;
        dX = 1;
        direction = "Northeast";

    }
    else if (dir == "nw")
    {
        dY = -1;
        dX = -1;
        direction = "Northwest";
    }

    else if (dir == "se")
    {
        dY = 1;
        dX = 1;
        direction = "Southeast";

    }

    else if (dir == "sw")
    {
        dY = 1;
        dX = -1;
        direction = "Southwest";

    }
    else
    {
        cerr << "Invalid direction" << endl;
    }
    return make_pair(make_pair(x + dX, y + dY), direction);
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

    // Check all gold
    for (auto gold : theGold)
    {
        if (gold->getX() == x && gold->getY() == y)
        {
            return true;
        }
    }

    // TODO: Check all enemy coordinates

    // Else no entity occupies this space, so return false
    return false;
}

// Generate a player in a random position in a chamber
void CC3K::generatePlayer()
{

    switch (startingRace)
    {
    case (Player::RaceTypes::SHADE):
    {
        thePlayer = make_shared<Shade>();
        break;
    }
    case (Player::RaceTypes::DROW):
    {
        thePlayer = make_shared<Drow>();
        break;
    }
    case (Player::RaceTypes::VAMPIRE):
    {
        thePlayer = make_shared<Vampire>();
        break;
    }
    case (Player::RaceTypes::GOBLIN):
    {
        thePlayer = make_shared<Goblin>();
        break;
    }
    case (Player::RaceTypes::TROLL):
    {
        thePlayer = make_shared<Troll>();
        break;
    }
    }

}

void CC3K::generateStairway()
{
    // Assumption: only the player has been generated at this point
    // Thus we only need to check collision with player
    theStairway = make_shared<Stairway>();

    int playerChamberNum = theFloor->chamberAt(thePlayer->getX(), thePlayer->getY());

    // Generate a random chamber number (to ensure each chamber has equal probability)
    // Ensure it is NOT EQUAL to player chamber
    int targetChamberNum = theFloor->getRandomChamberNum();
    ;
    while (targetChamberNum == playerChamberNum)
    {
        targetChamberNum = theFloor->getRandomChamberNum();
    }
    while (true)
    {
        // Get random coordinates and a random chamber number
        int randX = theFloor->getRandomX();
        int randY = theFloor->getRandomY();
        int chamberNum = theFloor->chamberAt(randX, randY);

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
        case Potion::PotionTypes::RESTOREHEALTH:
        {
            newPotion = make_shared<RestoreHealth>();
            break;
        }
        case Potion::PotionTypes::POISONHEALTH:
        {
            newPotion = make_shared<PoisonHealth>();
            break;
        }
        case Potion::PotionTypes::BOOSTATK:
        {
            newPotion = make_shared<BoostAtk>();
            break;
        }
        case Potion::PotionTypes::WOUNDATK:
        {
            newPotion = make_shared<WoundAtk>();
            break;
        }
        case Potion::PotionTypes::BOOSTDEF:
        {
            newPotion = make_shared<BoostDef>();
            break;
        }
        case Potion::PotionTypes::WOUNDDEF:
        {
            newPotion = make_shared<WoundDef>();
            break;
        }
        }

        // Generate a random chamber number (to ensure each chamber has equal probability)
        int targetChamberNum = theFloor->getRandomChamberNum();

        while (isGenerating)
        {
            // Get random coordinates and its associated chamber
            int randX = theFloor->getRandomX();
            int randY = theFloor->getRandomY();
            int chamberNum = theFloor->chamberAt(randX, randY);

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

void CC3K::generateGold()
{
    theGold.clear();

    for (int i = 0; i < NUM_GOLD; i++)
    {
        bool isGenerating = true;

        // Generate a number from 1-8
        // For the desired probability distribution
        // 5/8 normal, 1/8 dragon, 1/4 small hoard
        int goldType = rand() % 8 + 1;

        auto newGold = make_shared<Gold>("", 0);

        // 5/8 probability normal
        if (goldType <= 5)
        {
            newGold = make_shared<NormalPile>();
        }
        // 1/8 probability dragon
        else if (goldType <= 6)
        {
            newGold = make_shared<DragonHoard>();
        }
        // 1/4 probability small 
        else {
            newGold = make_shared<SmallPile>();
        }

        // Generate a random chamber number (to ensure each chamber has equal probability)
        int targetChamberNum = theFloor->getRandomChamberNum();

        while (isGenerating)
        {
            // Get random coordinates and its associated chamber
            int randX = theFloor->getRandomX();
            int randY = theFloor->getRandomY();
            int chamberNum = theFloor->chamberAt(randX, randY);

            if (chamberNum == targetChamberNum && !isOccupied(randX, randY))
            {
                newGold->setX(randX);
                newGold->setY(randY);
                theGold.push_back(newGold);
                isGenerating = false;
            }
        }
    }
}

void CC3K::movePlayer(string dir)
{
    pair<pair<int, int>,string> newPos = getPosAtDirection(thePlayer->getX(), thePlayer->getY(), dir);
    // Compute the new position

    int newX = newPos.first.first;
    int newY = newPos.first.second;

    // Check if the new position is a stairway FIRST
    // In this case, go to next level
    if (newX == theStairway->getX() && newY == theStairway->getY())
    {
        // Respawn everything
        newLevel();
        // Increase level number
        levelNum += 1;

        messages.emplace_back("Now entering level " + to_string(levelNum) + "!", Color::BOLDYELLOW);

        return;
    }

    // Next, check if player moved onto gold
    // Check if there exists a potion in the specified direction
    int foundGold = -1;
    for (int i = 0; i < theGold.size(); i++)
    {
        if (newX == theGold[i]->getX() && newY == theGold[i]->getY())
        {
            foundGold = i;
            break;
        }
    }
    // If the player is walking on gold
    if (foundGold != -1)
    {
        // TODO: Check dragon hoard logic

        // Add the gold to player
        playerGold += theGold[foundGold]->getValue();

        // Add the message
        messages.emplace_back("PC picks up a " + 
            theGold[foundGold]->getName() + " worth " + 
            to_string(theGold[foundGold]->getValue()) + " gold.", 
            Color::GREEN);


        // Remove the gold from the map
        theGold.erase(theGold.begin()+foundGold);

        // Move the player over the gold
        thePlayer->setX(newX);
        thePlayer->setY(newY);

        return;
    }
    
    // Else, not a stairway or gold, we can use isOccupied() properly
    // Check if the new position is a tile
    // (i.e. don't move into a wall or empty space)
    if (isOccupied(newX, newY) ||
        (theFloor->cellAt(newX, newY).getChar() != Cell::TILE &&
         theFloor->cellAt(newX, newY).getChar() != Cell::DOOR &&
         theFloor->cellAt(newX, newY).getChar() != Cell::PASSAGE))
    {
        messages.emplace_back("You cannot move there.", Color::RED);
        return;
    }

    // Else the new position is valid, move player there
    thePlayer->setX(newX);
    thePlayer->setY(newY);
    messages.emplace_back("PC moves " + newPos.second + ".", Color::BOLDMAGENTA);


}

// Use a potion in the direction specified (from the player)
void CC3K::usePotion(string dir)
{
    // Get the coordinates to check
    pair<pair<int, int>,string> checkPotion = getPosAtDirection(thePlayer->getX(), thePlayer->getY(), dir);
    pair<int,int> checkPotionPos = checkPotion.first;

    // Check if there exists a potion in the specified direction
    int found = -1;
    for (int i = 0; i < thePotions.size(); i++)
    {
        if (thePotions[i]->getX() == checkPotionPos.first && thePotions[i]->getY() == checkPotionPos.second)
        {
            found = i;
            break;
        }
    }

    // If we did not find a potion at that location
    if (found == -1)
    {
        messages.emplace_back("There is no potion in that direction.", Color::RED);
        return;
    }
    // Potion is found, apply the potion and erase it from the potion vector
    else
    {
        // Apply the effect to the player
        thePlayer->applyPotion(thePotions[found]);

        // Output message
        // E.g. "You used a Potion of Restore Health"
        messages.emplace_back("You used a " + thePotions[found]->getName() + ". " + thePotions[found]->getDescription(),
                            Color::BOLDMAGENTA);


        // Erase the potion from the vector
        thePotions.erase(thePotions.begin() + found);
    }
}
void CC3K::render()
{
    notifyObservers();
    // Clear the messages
    messages.clear();
}

string CC3K::getGameStatus()
{
    string ret = "";
    ret += "Race: " + thePlayer->getName() + " ";

    // Print out player gold
    ret += "Gold: " + to_string(playerGold) + "\n";

    // Print out HP
    ret += "HP: " + to_string(thePlayer->getHP()) + "\n";

    // Print out Atk
    ret += "Atk: " + to_string(thePlayer->getAtk()) + "\n";

    // Print out Def
    ret += "Def: " + to_string(thePlayer->getDef()) + "\n";

    // Print out action (for any actions) that occur
    ret += "Action:";

    return ret;
}

pair<char,string> CC3K::getState(int x, int y)
{
    // Check the player coordinates
    if (thePlayer->getX() == x && thePlayer->getY() == y)
    {
        return make_pair(thePlayer->getSymbol(), thePlayer->getColor());
    }

    // Check the staircase coordinates
    if (theStairway->getX() == x && theStairway->getY() == y)
    {
        return make_pair(theStairway->getSymbol(),theStairway->getColor()) ;
    }

    // Check all potions
    for (auto potion : thePotions)
    {
        if (potion->getX() == x && potion->getY() == y)
        {
            return make_pair(potion->getSymbol(),potion->getColor()) ;
        }
    }

    // TODO: Check gold
    // Check all gold
    for (auto gold : theGold)
    {
        if (gold->getX() == x && gold->getY() == y)
        {
            return make_pair(gold->getSymbol(), gold->getColor());
        }
    }
    // TODO: Check enemies

    // Else, return the floor element
    return make_pair(theFloor->cellAt(x, y).getChar(), Color::RESET);
}

void CC3K::setStartingRace(int newRace)
{
    startingRace = newRace;
    messages.emplace_back("Switched race.", Color::BOLDCYAN);
}

vector<Message> CC3K::getMessages() {
    return messages;
}

void CC3K::display()
{
    // DEPRECATED (use TextObserver and GraphicalObserver and CC3K->render())
    // /* Rendering algorithm:
    //  *   Copy over the environment to the display (which will overwrite the old display)
    //  *   Loop through all entities and overwrite the display with each entity's position
    //  *   Print out the new display
    //  */

    // theDisplay = theFloor->getEnvironmentChar();

    // // Place the player's position
    // theDisplay[thePlayer->getY()][thePlayer->getX()] = thePlayer->getSymbol();

    // // Draw the staircase
    // theDisplay[theStairway->getY()][theStairway->getX()] = theStairway->getSymbol();

    // // Draw the potions
    // for (auto potion : thePotions)
    // {
    //     theDisplay[potion->getY()][potion->getX()] = potion->getSymbol();
    // }

    // // TODO: Draw the gold piles

    // // TODO: Draw enemies

    // // Print out the display
    // for (int i = 0; i < theDisplay.size(); i++)
    // {
    //     for (int j = 0; j < theDisplay[0].size(); j++)
    //     {
    //         cout << theDisplay[i][j];
    //     }
    //     cout << endl;
    // }

    // // Print out race
    // cout << "Race: " << thePlayer->getName();

    // // Print out player gold
    // cout << " Gold: " << playerGold << endl;

    // // Print out HP
    // cout << "HP: " << thePlayer->getHP() << endl;

    // // Print out Atk
    // cout << "Atk: " << thePlayer->getAtk() << endl;

    // // Print out Def
    // cout << "Def: " << thePlayer->getDef() << endl;

    // // Print out action (for any actions) that occur
    // cout << "Action: " << endl;
}
