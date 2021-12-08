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

shared_ptr<Potion> SpawnCreator::createPotionAt(int potionType, int x, int y) {
	shared_ptr<Potion> newPotion = createPotion(potionType);

	newPotion->setX(x);
	newPotion->setY(y);
	return newPotion;
}

shared_ptr<Gold> SpawnCreator::createGoldAt(int goldType, int x, int y, std::shared_ptr<Dragon> dragon) {

	shared_ptr<Gold> newGold = nullptr;
	if (goldType == Gold::GoldTypes::DRAGON_HOARD)
	{
		if (dragon == nullptr) {
			throw invalid_argument("argument DragonHoard for gold_type requires an associated dragon");
		}
		int numAttempts = 0; // may fail due to associated dragon
		while (true)
		{
			auto dragonGold = make_shared<DragonHoard>();
			auto theDragon = make_shared<Dragon>(dragonGold);

			dragonGold->setX(x);
			dragonGold->setY(y);

			// Generate a dx and dy either -1 or 1
			int dY = rand() % 2 ? -1 : 1;
			int dX = rand() % 2 ? -1 : 1;

			int dragonX;
			int dragonY;
			// Too many failed generation attempts, just spawn somewhere
			if (numAttempts > 10)
			{
				dragonX = theFloor->getRandomX();
				dragonY = theFloor->getRandomY();
			}
			else {
			dragonX = dX + x;
			dragonY = dY + y;

			}

			// Set the coordinates for gold and dragon
			if (!(game.isOccupied(dragonX, dragonY)) && theFloor->chamberAt(dragonX, dragonY) == theFloor->chamberAt(x, y) && dragon != nullptr)
			{
				dragon->setHoard(dragonGold);
				dragon->setX(dragonX);
				dragon->setY(dragonY);
				return dragonGold;
			}
		}
	}
	createGold(goldType);

	newGold->setX(x);
	newGold->setY(y);
	return newGold;
}
