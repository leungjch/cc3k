#include "spawncreator.h"

#include "cc3k.h"
#include "floor.h"

#include "potion/potion.h"
#include "treasure/gold.h"
#include "treasure/dragonHoard.h"
#include "enemy/dragon.h"

using namespace std;

SpawnCreator::SpawnCreator(shared_ptr<Floor> floorMap, const CC3K &game)
	: LocalizedCreator{floorMap}, game{game} {}
SpawnCreator::~SpawnCreator() {};

shared_ptr<Potion> SpawnCreator::createPotionAt(shared_ptr<Potion> newPotion, int x, int y) {
	newPotion->setX(x);
	newPotion->setY(y);
	return newPotion;
}

// requires: newGold is a DragonHoard if dragon is not nullptr
shared_ptr<Gold> SpawnCreator::createGoldAt(shared_ptr<Gold> newGold, int x, int y, std::shared_ptr<Dragon> dragon) {
	newGold->setX(x);
	newGold->setY(y);
	if (dragon != nullptr)
	{
		shared_ptr<DragonHoard> dragonHoard = dynamic_pointer_cast<DragonHoard>(newGold);
		if (dragonHoard == nullptr) {
			throw invalid_argument("a non-null dragon requires requires an associated DragonHoard");
		}

		int numAttempts = 0; // may fail due to associated dragon
		while (true)
		{
			auto theDragon = make_shared<Dragon>(dragonHoard);

			int dragonX;
			int dragonY;
			// Too many failed generation attempts, just spawn somewhere
			if (numAttempts > 10)
			{
				dragonX = theFloor->getRandomX();
				dragonY = theFloor->getRandomY();
			}
			else
			{
				// Generate a dx and dy either -1 or 1
				int dY = rand() % 2 ? -1 : 1;
				int dX = rand() % 2 ? -1 : 1;

				dragonX = dX + x;
				dragonY = dY + y;
			}

			int chamberNum = theFloor->chamberAt(dragonX, dragonY);
			int targetChamberNum = theFloor->chamberAt(x, y);
			// Set the coordinates for gold and dragon
			if (!game.isOccupied(dragonX, dragonY) && (chamberNum == targetChamberNum) && (dragon != nullptr))
			{
				dragon->setHoard(dragonHoard);
				dragon->setX(dragonX);
				dragon->setY(dragonY);
				return dragonHoard;
			}
		}
	}

	return newGold;
}
