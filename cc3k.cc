#include "cc3k.h"
#include "player/player.h"
#include "player/drow.h"
#include "player/shade.h"
#include "player/goblin.h"
#include "player/troll.h"
#include "player/vampire.h"

#include "enemy/enemy.h"
#include "enemy/orc.h"
#include "enemy/human.h"
#include "enemy/dragon.h"
#include "enemy/dwarf.h"
#include "enemy/halfling.h"
#include "enemy/elf.h"
#include "enemy/merchant.h"

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
#include <map>

using namespace std;

CC3K::CC3K() : levelNum{1}, playerGold{0}, startingRace{Player::RaceTypes::SHADE}, stopEnemies{false}, isHostileMerchants{false},
               theFloor{make_shared<Floor>()}
{
}

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
    thePotions.clear();
    theGold.clear();
    theEnemies.clear();

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
    generateEnemies();
}

void CC3K::placePlayer()
{
    // Select a random chamber number

    while (true)
    {
        int targetChamberNum = theFloor->getRandomChamberNum();
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

    // Check all enemy coordinates
    for (auto enemy : theEnemies)
    {
        if (enemy->getX() == x && enemy->getY() == y)
        {
            return true;
        }
    }

    // Else no entity occupies this space, so return false
    return false;
}

// Generate a player in a random position in a chamber
void CC3K::generatePlayer()
{
    messages.emplace_back("Player character has spawned. ", Color::GREEN);

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

    while (true)
    {
        // Generate a random chamber number (to ensure each chamber has equal probability)
        // Ensure it is NOT EQUAL to player chamber
        int targetChamberNum = theFloor->getRandomChamberNum();

        while (targetChamberNum == playerChamberNum)
        {
            targetChamberNum = theFloor->getRandomChamberNum();
        }

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

        while (isGenerating)
        {
            // Generate a random chamber number (to ensure each chamber has equal probability)
            int targetChamberNum = theFloor->getRandomChamberNum();

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

    for (int i = 0; i < NUM_GOLD; i++)
    {
        bool isGenerating = true;

        // Generate a number from 1-8
        // For the desired probability distribution
        // 5/8 normal, 1/8 dragon, 1/4 small hoard
        int goldType = rand() % 8 + 1;

        auto newGold = make_shared<Gold>("", 0);

        // If we are spawning a dragonhoard
        bool isDragonHoard = false;

        // 5/8 probability normal
        if (goldType <= 5)
        {
            newGold = make_shared<NormalPile>();
        }
        // 1/8 probability dragon
        else if (goldType <= 6)
        {
            // Spawn a dragon later
            isDragonHoard = true;
        }
        // 1/4 probability small
        else
        {
            newGold = make_shared<SmallPile>();
        }

        while (isGenerating)
        {
            // Generate a random chamber number (to ensure each chamber has equal probability)
            int targetChamberNum = theFloor->getRandomChamberNum();

            // Get random coordinates and its associated chamber
            int randX = theFloor->getRandomX();
            int randY = theFloor->getRandomY();
            int chamberNum = theFloor->chamberAt(randX, randY);
            if (chamberNum == targetChamberNum && !isOccupied(randX, randY))
            {
                if (isDragonHoard)
                {
                    // Generate random coordinates for the dragon
                    auto dragonGold = make_shared<DragonHoard>();

                    // Generate a dragon with the associated dragon hoard
                    auto dragon = make_shared<Dragon>(dragonGold);

                    // Generate the coordinates for the dragon

                    // Generate a dx and dy either -1 or 1
                    int dY = rand() % 2 ? -1 : 1;
                    int dX = rand() % 2 ? -1 : 1;

                    int dragonX = dX + randX;
                    int dragonY = dY + randY;

                    // Set the coordinates for gold and dragon
                    dragonGold->setX(randX);
                    dragonGold->setY(randY);
                    dragon->setX(dragonX);
                    dragon->setY(dragonY);

                    int dragonChamberNum = theFloor->chamberAt(dragonX, dragonY);

                    if (dragonChamberNum == targetChamberNum && !isOccupied(dragonX, dragonY))
                    {

                        theGold.push_back(dragonGold);

                        theEnemies.push_back(dragon);

                        isGenerating = false; // exit the loop
                    }
                }
                else
                {
                    newGold->setX(randX);
                    newGold->setY(randY);
                    theGold.push_back(newGold);
                    isGenerating = false; // exit the loop
                }
                // If generation fails, we do nothing
            }
        }
    }
}

void CC3K::checkPlayerDead()
{
    if (thePlayer->getHP() <= 0)
    {
        messages.emplace_back("You died!", Color::RED);
    }
}

void CC3K::generateEnemies()
{
    for (int i = 0; i < NUM_GOLD; i++)
    {
        bool isGenerating = true;

        // Generate a number from 1-18
        // For the desired probability distribution
        int enemyType = rand() % 18 + 1;

        auto newEnemy = make_shared<Enemy>(0, 0, 0, false, 'E', "");

        // Cumulative probability
        // 2/9 human
        if (enemyType <= 4)
        {
            newEnemy = make_shared<Human>();
        }
        //
        else if (enemyType <= 7)
        {
            newEnemy = make_shared<Dwarf>();
        }
        else if (enemyType <= 12)
        {
            newEnemy = make_shared<Halfling>();
        }
        else if (enemyType <= 14)
        {
            newEnemy = make_shared<Elf>();
        }
        else if (enemyType <= 16)
        {
            newEnemy = make_shared<Orc>();
        }
        else if (enemyType <= 18)
        {
            newEnemy = make_shared<Merchant>(isHostileMerchants);
        }
        else
        {
            cerr << "Error generating enemie" << endl;
        }

        while (isGenerating)
        {
            // Generate a random chamber number (to ensure each chamber has equal probability)
            int targetChamberNum = theFloor->getRandomChamberNum();

            // Get random coordinates and its associated chamber
            int randX = theFloor->getRandomX();
            int randY = theFloor->getRandomY();
            int chamberNum = theFloor->chamberAt(randX, randY);

            if (chamberNum == targetChamberNum && !isOccupied(randX, randY))
            {
                newEnemy->setX(randX);
                newEnemy->setY(randY);
                theEnemies.push_back(newEnemy);
                isGenerating = false;
            }
        }
    }
}

void CC3K::movePlayer(string dir)
{
    pair<pair<int, int>, string> newPos = getPosAtDirection(thePlayer->getX(), thePlayer->getY(), dir);
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
        messages.emplace_back("PC moves " + newPos.second + ".", Color::BOLDMAGENTA);

        // Check dragon hoard
        // The condition will be true whenever gold is not a dragon hoard
        // Or when it is a dragon hoard AND the associated dragon is slain

        if (!(theGold[foundGold]->getName() == "Dragon Hoard" && !theGold[foundGold]->getPickup()))
        {
            // If normal gold, we collect it and delete it

            // Add the gold to player
            playerGold += theGold[foundGold]->getValue();

            // Add the message
            messages.emplace_back("PC picks up a " +
                                      theGold[foundGold]->getName() + " worth " +
                                      to_string(theGold[foundGold]->getValue()) + " gold.",
                                  Color::GREEN);

            // Remove the gold from the map
            theGold.erase(theGold.begin() + foundGold);
        }
        // Move the player over the gold
        thePlayer->setX(newX);
        thePlayer->setY(newY);

        // The enemies now move and attack the player if in range
        moveAndAttackEnemies();

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
        // The enemies now move and attack the player if in range
        moveAndAttackEnemies();

        return;
    }

    // Else the new position is valid, move player there
    thePlayer->setX(newX);
    thePlayer->setY(newY);
    messages.emplace_back("PC moves " + newPos.second + ".", Color::BOLDMAGENTA);

    // The enemies now move and attack the player if in range
    moveAndAttackEnemies();
}

// Use a potion in the direction specified (from the player)
void CC3K::usePotion(string dir)
{
    // Get the coordinates to check
    pair<pair<int, int>, string> checkPotion = getPosAtDirection(thePlayer->getX(), thePlayer->getY(), dir);
    pair<int, int> checkPotionPos = checkPotion.first;

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
        messages.emplace_back("PC used a " + thePotions[found]->getName() + ". " + thePotions[found]->getDescription(),
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

pair<char, string> CC3K::getState(int x, int y)
{
    // Check the player coordinates
    if (thePlayer->getX() == x && thePlayer->getY() == y)
    {
        return make_pair(thePlayer->getSymbol(), thePlayer->getColor());
    }

    // Check the staircase coordinates
    if (theStairway->getX() == x && theStairway->getY() == y)
    {
        return make_pair(theStairway->getSymbol(), theStairway->getColor());
    }

    // Check all potions
    for (auto potion : thePotions)
    {
        if (potion->getX() == x && potion->getY() == y)
        {
            return make_pair(potion->getSymbol(), potion->getColor());
        }
    }

    // Check all gold
    for (auto gold : theGold)
    {
        if (gold->getX() == x && gold->getY() == y)
        {
            return make_pair(gold->getSymbol(), gold->getColor());
        }
    }
    // Check enemies
    for (auto enemy : theEnemies)
    {
        if (enemy->getX() == x && enemy->getY() == y)
        {
            return make_pair(enemy->getSymbol(), enemy->getColor());
        }
    }
    // Else, return the floor element
    return make_pair(theFloor->cellAt(x, y).getChar(), Color::RESET);
}

void CC3K::toggleStopEnemies()
{
    stopEnemies = !stopEnemies;
    if (stopEnemies)
    {
        messages.emplace_back("Enemies are frozen now!", Color::BLUE);
    }
    else
    {
        messages.emplace_back("Enemies are unfrozen now!", Color::BLUE);
    }
}

// Move all the enemies and attack the player if they are in range
void CC3K::moveAndAttackEnemies()
{
    if (stopEnemies)
    {
        return;
    }

    // "starting at the leftmost enemy, move all enemies on that row and then move
    // to the next row starting with the leftmost. Any particular enemy should only be moved once per player action (e.g. moving
    // to a line that has not been processed does not grant an extra move)."

    // This makes the implementation more complicated than simply looping through the vector of enemies

    vector<shared_ptr<Enemy>> enemiesCopy = theEnemies;

    // Create a hash map to mark the xy coordinates of the new enemy locations
    std::map<std::pair<int, int>, bool> checkXY;

    for (int i = 0; i < theFloor->getHeight(); i++)
    {
        for (int j = 0; j < theFloor->getWidth(); j++)
        {
            // Skip non-chamber tiles since enemies cannot be there
            if (theFloor->chamberAt(j, i) == -1)
            {
                continue;
            }

            // Else search for an enemy
            else
            {
                for (int k = 0; k < theEnemies.size(); k++)
                {
                    if (checkXY.count(make_pair(j, i)) == 0 && i == theEnemies[k]->getY() && j == theEnemies[k]->getX())
                    {
                        moveAndAttackEnemy(theEnemies[k]);
                        checkXY[make_pair(theEnemies[k]->getY(), theEnemies[k]->getX())] = true;
                    }
                }
            }
        }
    }

    // Check if the player has died from enemy attacks
    checkPlayerDead();
}

void CC3K::moveAndAttackEnemy(shared_ptr<Enemy> enemy)
{

    // Move the enemy
    // Generate a random -1, 0, 1
    int deltaX = rand() % 3 - 1;
    int deltaY = rand() % 3 - 1;
    int newX = enemy->getX() + deltaX;
    int newY = enemy->getY() + deltaY;
    if (!isOccupied(newX, newY) && theFloor->cellAt(newX, newY).getChar() == Cell::TILE)
    {
        enemy->move(deltaX, deltaY);
    }

    // Attack the player if they are in range

    // If merchant and not triggered hostile, skip (don't attack)
    if (enemy->getName() == "Merchant" && !isHostileMerchants)
    {
        return;
    }
    if (enemy->inRange(thePlayer))
    {
        // Number of times that the enemy can attack the player
        // Default is 1, 2 if enemy is elf
        int numAttacks = 1;
        if (enemy->getName() == "Elf" && thePlayer->getName() != "Drow")
        {
            numAttacks = 2;
        }
        for (int numAttack = 0; numAttack < numAttacks; numAttack++)
        {
            // Attack the pc if it is in range
            int dmg = enemy->attack(thePlayer);
            if (dmg == 0)
            {
                messages.emplace_back(enemy->getName() + " tried to attack PC but missed.", Color::RED);
            }
            else
            {
                messages.emplace_back(enemy->getName() + " deals " + to_string(dmg) + " damage to PC.", Color::RED);
            }
        }
    }
}

void CC3K::playerAttack(string cmd)
{
    pair<pair<int, int>, string> pos = getPosAtDirection(thePlayer->getX(), thePlayer->getY(), cmd);
    // The coordinates where the player is striking the possible enemy
    int attackX = pos.first.first;
    int attackY = pos.first.second;

    // To keep track of if player hit an enemy
    int haveHit = false;

    for (int i = 0; i < theEnemies.size(); i++)
    {
        // Check if the player is in range of attacking the enemy
        if (theEnemies[i]->inRange(thePlayer) && theEnemies[i]->getX() == attackX && theEnemies[i]->getY() == attackY)
        {
            int dmg = thePlayer->attack(theEnemies[i]);
            haveHit = true;

            // Extra logic for vampire: If successful attack, it gains 5hp
            if (thePlayer->getName() == "Vampire" && dmg != 0)
            {
                if (theEnemies[i]->getName() == "Dwarf")
                {
                    messages.emplace_back("PC is allergic to Dwarf and lost 5 HP.", Color::BLUE);
                }
                else
                {
                    messages.emplace_back("PC gains 5 HP as a vampire from the successful attack.", Color::BLUE);
                }
            }

            // If player misses (this is possible if halfling attacks player)
            if (dmg == 0)
            {
                messages.emplace_back("PC attacked but missed.", Color::MAGENTA);
            }

            // If the enemy's HP drops below 0, it is dead
            else if (theEnemies[i]->getHP() <= 0)
            {
                // Extra logic for merchant
                if (theEnemies[i]->getName() == "Merchant" && !isHostileMerchants)
                {
                    // All merchants are hostile now
                    isHostileMerchants = true;
                    messages.emplace_back("PC has slain " + theEnemies[i]->getName() + " (" + to_string(dmg) + " damage).", Color::YELLOW);
                    messages.emplace_back("You have drawn the ire of all merchants. They will be hostile to you from now on!", Color::MAGENTA);
                }
                // Extra logic for dragon: allow pickup the gold
                else if (theEnemies[i]->getName() == "Dragon")
                {
                    // Get the gold
                    Dragon &theDragon = dynamic_cast<Dragon &>(*theEnemies[i]);
                    auto dragonGold = theDragon.getHoard();

                    // Set the gold to allow pickup
                    dragonGold->setPickup(true);
                    messages.emplace_back("PC has slain " + theEnemies[i]->getName() + " (" + to_string(dmg) + " damage).", Color::YELLOW);
                    messages.emplace_back("The dragon's hoard is now yours to take. ", Color::GREEN);
                }
                // Otherwise standard enemy was slain
                else
                {
                    messages.emplace_back("PC has slain " + theEnemies[i]->getName() + " (" + to_string(dmg) + " damage).", Color::YELLOW);
                }

                // Humans drop two normal piles of gold
                if (theEnemies[i]->getName() == "Human")
                {
                    spawnGoldPileAt(Gold::GoldTypes::MEDIUM, theEnemies[i]->getX(), theEnemies[i]->getY());
                    spawnGoldPileAt(Gold::GoldTypes::MEDIUM, theEnemies[i]->getX(), theEnemies[i]->getY());
                    messages.emplace_back("The slain Human dropped two normal piles of gold.", Color::GREEN);

                }
                if (theEnemies[i]->getName() == "Merchant")
                {
                    spawnGoldPileAt(Gold::GoldTypes::MERCHANT_HOARD, theEnemies[i]->getX(), theEnemies[i]->getY());
                    messages.emplace_back("The slain Merchant dropped one Merchant Hoard.", Color::GREEN);
                }

                // Upon their demise, any enemy that is not a dragon, human, or merchant will drop either a small pile or normal pile of gold.
                // This gold is immediately added to the player character’s total.
                if (theEnemies[i]->getName() != "Dragon" && theEnemies[i]->getName() != "Human" && theEnemies[i]->getName() != "Merchant")
                {
                    int goldCollect = SmallPile::value;
                    if (rand() % 2)
                    {
                        goldCollect = NormalPile::value;
                    }

                    playerGold += goldCollect;

                    messages.emplace_back("PC collected " + to_string(goldCollect) + " gold from the slain " + theEnemies[i]->getName() + ".", Color::YELLOW);
                }

                // Goblin steals 5 gold from every slain enemy.
                if (thePlayer->getName() == "Goblin")
                {
                    playerGold += 5;
                    messages.emplace_back("PC's Goblin skills collect an extra 5 gold from the slain " + theEnemies[i]->getName() + ".", Color::YELLOW);
                }

                theEnemies[i] = theEnemies.back();
                theEnemies.pop_back();
            }

            // Player attacks enemy but enemy did not die yet
            else
            {
                messages.emplace_back("PC deals " + to_string(dmg) + " damage to " +
                                          theEnemies[i]->getName() + " (" + to_string(theEnemies[i]->getHP()) +
                                          " HP)" + ".",
                                      Color::CYAN);
            }
        }
    }
    if (!haveHit)
    {
        messages.emplace_back("There is nothing to attack there.", Color::BOLDMAGENTA);
    }

    moveAndAttackEnemies();
}

void CC3K::setStartingRace(int newRace)
{
    startingRace = newRace;
}

vector<Message> CC3K::getMessages()
{
    return messages;
}

void CC3K::addMessage(string text, string color)
{
    messages.emplace_back(text, color);
}

// Drop a gold pile around a source location
void CC3K::spawnGoldPileAt(int goldType, int sourceX, int sourceY)
{
    // Handling a rare edge case:
    // It is possible that the source location is too crowded and there is nowhere for the gold to spawn
    // If, after failed 10 attempts to spawn, we will just spawn the gold somewhere
    // else in the chamber
    int spawnAttempts = 0;

    auto newGold = make_shared<Gold>("", 0); 

    switch (goldType)
    {
        case (Gold::GoldTypes::MERCHANT_HOARD):
        {
            newGold = make_shared<MerchantHoard>();
            break;
        }
        case (Gold::GoldTypes::MEDIUM):
        {
            newGold = make_shared<NormalPile>();
            break;
        }
        default:
        {
            cerr << "Invalid gold type spawn." << endl;
            return;
        }
    }
    bool isGenerating = true;
    while (isGenerating)
    {

        int targetChamberNum = theFloor->chamberAt(sourceX, sourceY);

        int randX;
        int randY;
        int deltaX;
        int deltaY;

        // The source location is too crowded, spawn it somewhere in the chamber
        if (spawnAttempts > 100)
        {
            randX = theFloor->getRandomX();
            randY = theFloor->getRandomY();
        }
        else 
        {
            // Generate a random -1, 0, 1
            deltaX = rand() % 3 - 1;
            deltaY = rand() % 3 - 1;

            // Get random coordinates from the source
            randX = sourceX + deltaX;
            randY = sourceY + deltaY;
        }

        // Check the associated chamber to the random coordinates
        int chamberNum = theFloor->chamberAt(randX, randY);

        if (chamberNum == targetChamberNum && ((randX == sourceX && randY == sourceY) || (!isOccupied(randX, randY))))
        {
            newGold->setX(randX);
            newGold->setY(randY);
            theGold.push_back(newGold);
            isGenerating = false; // exit the loop
            // If generation fails, we do nothing
        }

        spawnAttempts += 1;
    }
}
