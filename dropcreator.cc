#include "dropcreator.h"

#include "cc3k.h"
#include "floor.h"

#include "entity.h"
#include "potion/potion.h"
#include "treasure/gold.h"
#include "treasure/dragonHoard.h"
#include "enemy/dragon.h"

using namespace std;

DropCreator::DropCreator(shared_ptr<Floor> floorMap, const CC3K &game)
	: LocalizedCreator{floorMap}, game{game} {}
DropCreator::~DropCreator() {};


shared_ptr<Potion> DropCreator::createPotionAt(shared_ptr<Potion> newPotion, int x, int y) {
	createEntityAt(newPotion, x, y);
	return newPotion;
}

// requires: dragon is nullptr and newGold is not a DragonHoard
// Drop a gold pile around a source location
shared_ptr<Gold> DropCreator::createGoldAt(shared_ptr<Gold> newGold, int x, int y, std::shared_ptr<Dragon> dragon) {
	createEntityAt(newGold, x, y);
	return newGold;
}

void DropCreator::createEntityAt(shared_ptr<Entity> newEntity, int x, int y) {
    // Handling a rare edge case:
    // It is possible that the source location is too crowded and there is nowhere for the gold to spawn
    // If, after failed 10 attempts to spawn, we will just spawn the gold somewhere
    // else in the chamber
    int spawnAttempts = 0;
    while (true)
    {
        int randX;
        int randY;

        // The source location is too crowded, spawn it somewhere in the chamber
        if (spawnAttempts > 100)
        {
            randX = theFloor->getRandomX();
            randY = theFloor->getRandomY();
        }
        else
        {
            // Generate a random -1, 0, 1
            int deltaX = rand() % 3 - 1;
            int deltaY = rand() % 3 - 1;

            // Get random coordinates from the source
            randX = x + deltaX;
            randY = x + deltaY;
        }

        // Check the associated chamber to the random coordinates
        int chamberNum = theFloor->chamberAt(randX, randY);
        int targetChamberNum = theFloor->chamberAt(x, y);

        if (chamberNum == targetChamberNum && ((randX == x && randY == y) || (!game.isOccupied(randX, randY))))
        {
            newEntity->setX(randX);
            newEntity->setY(randY);
            return;
            // If generation fails, we do nothing
        }

        spawnAttempts += 1;
    }
}
