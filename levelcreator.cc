#include "levelcreator.h"

#include "cc3k.h"

#include <iostream>

#include "player/player.h"
#include "enemy/enemy.h"
#include "enemy/dragon.h"
#include "potion/potion.h"
#include "treasure/gold.h"
#include "treasure/dragonHoard.h"

#include "stairway.h"
#include "floor.h"

using namespace std;

LevelCreator::LevelCreator(shared_ptr<Floor> floorMap, const CC3K &game)
	: GeneralCreator{floorMap}, game{game} {}

/*
 * Generates a player according to startingRace.
 */
shared_ptr<Player> LevelCreator::generatePlayer(int startingRace)
{
	shared_ptr<Player> newPlayer = createPlayer(startingRace);
	placePlayer(newPlayer);

	return newPlayer;
}

shared_ptr<Stairway> LevelCreator::generateStairway(int playerChamberNum)
{
	// Assumption: only the player has been generated at this point
	// Thus we only need to check collision with player
	shared_ptr<Stairway> newStairway = createStairway();

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
		if (!game.isOccupied(randX, randY) && (chamberNum == targetChamberNum) && (chamberNum != playerChamberNum))
		{
			newStairway->setX(randX);
			newStairway->setY(randY);
			return newStairway;
		}
	}
}

shared_ptr<Potion> LevelCreator::generatePotion()
{
	// Generate a random potion type
	int potionType = rand() % Potion::NUM_POTION_TYPES + 1;

	shared_ptr<Potion> newPotion = createPotion(potionType);
	while (true)
	{
		// Generate a random chamber number (to ensure each chamber has equal probability)
		int targetChamberNum = theFloor->getRandomChamberNum();

		// Get random coordinates and its associated chamber
		int randX = theFloor->getRandomX();
		int randY = theFloor->getRandomY();
		int chamberNum = theFloor->chamberAt(randX, randY);

		if ((chamberNum == targetChamberNum) && !(game.isOccupied(randX, randY)))
		{
			newPotion->setX(randX);
			newPotion->setY(randY);
			return newPotion;
		}
	}
	return nullptr;
}

shared_ptr<Gold> LevelCreator::generateGold(shared_ptr<Dragon> dragonEnemy)
{
	bool isDragonHoard = false; // if we are spawning a dragonhoard
	std::shared_ptr<Gold> newGold = nullptr;
	int goldType = getRandomType({5, 1, 2}, 8);
	if (goldType == 2)
	{
		isDragonHoard = true;
	}
	else
	{
		newGold = createGold(goldType);
	}

	while (true)
	{
		// Generate a random chamber number (to ensure each chamber has equal probability)
		int targetChamberNum = theFloor->getRandomChamberNum();

		// Get random coordinates and its associated chamber
		int randX = theFloor->getRandomX();
		int randY = theFloor->getRandomY();
		int chamberNum = theFloor->chamberAt(randX, randY);
		if ((chamberNum == targetChamberNum) && !game.isOccupied(randX, randY))
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

				int dragonChamberNum = theFloor->chamberAt(dragonX, dragonY);

				if (dragonChamberNum == targetChamberNum && !game.isOccupied(dragonX, dragonY))
				{
					dragonEnemy->setX(dragonX);
					dragonEnemy->setY(dragonY);
					dragonEnemy->setHoard(dragonGold);

					return dragonGold;
				}
			}
			else
			{
				newGold->setX(randX);
				newGold->setY(randY);
				dragonEnemy->setHoard(nullptr);
				return newGold;
			}
			// If generation fails, we do nothing
		}
	}
	return nullptr;
}

shared_ptr<Enemy> LevelCreator::generateEnemy(bool hostileMerchants)
{
	shared_ptr<Enemy> newEnemy = nullptr;
	int enemyType;
	if (game.getDLC()) {
		enemyType = getRandomType({2, 2, 2, 2, 2, 4, 4}, 18);
	} else {
		enemyType = getRandomType({4, 3, 5, 2, 2, 2}, 18);
	}
	newEnemy = createEnemy(enemyType, hostileMerchants);

	while (true)
	{
		// Generate a random chamber number (to ensure each chamber has equal probability)
		int targetChamberNum = theFloor->getRandomChamberNum();

		// Get random coordinates and its associated chamber
		int randX = theFloor->getRandomX();
		int randY = theFloor->getRandomY();
		int chamberNum = theFloor->chamberAt(randX, randY);

		if (chamberNum == targetChamberNum && !game.isOccupied(randX, randY))
		{
			newEnemy->setX(randX);
			newEnemy->setY(randY);

			return newEnemy;
		}
	}
	return nullptr;
}

void LevelCreator::placePlayer(shared_ptr<Player> thePlayer)
{
	while (true)
	{
		int targetChamberNum = theFloor->getRandomChamberNum(); // select a random chamber number
		// Get random coordinates
		int randX = theFloor->getRandomX();
		int randY = theFloor->getRandomY();
		int chamberNum = theFloor->chamberAt(randX, randY);

		// Check the chamber
		if (!game.isOccupied(randX, randY) && chamberNum == targetChamberNum)
		{
			thePlayer->setX(randX);
			thePlayer->setY(randY);
			break;
		}
	}
}

