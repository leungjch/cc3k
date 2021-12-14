#ifndef TREASURE_DROPCREATOR_H_
#define TREASURE_DROPCREATOR_H_

#include <memory>
#include "localizedcreator.h"

class CC3K;
class Entity;
class Floor;
class Potion;
class Gold;
class Dragon;

class DropCreator: public LocalizedCreator {
	const CC3K &game;

	void createEntityAt(std::shared_ptr<Entity> newEntity, int x, int y);
public:
	DropCreator(std::shared_ptr<Floor> floorMap, const CC3K &game);

    std::shared_ptr<Potion> createPotionAt(std::shared_ptr<Potion> newPotion, int x, int y) override;
    std::shared_ptr<Gold> createGoldAt(std::shared_ptr<Gold> newGold, int x, int y,
    		std::shared_ptr<Dragon> dragon) override;

    ~DropCreator();
};



#endif /* TREASURE_DROPCREATOR_H_ */
