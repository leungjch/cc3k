#ifndef SPAWNCREATOR_H_
#define SPAWNCREATOR_H_

#include <memory>
#include "localizedcreator.h"

class CC3K;
class Floor;
class Potion;
class Gold;
class Dragon;

class SpawnCreator: public LocalizedCreator {
	const CC3K &game;
public:
	SpawnCreator(std::shared_ptr<Floor> floorMap, const CC3K &game);

    std::shared_ptr<Potion> createPotionAt(int potionType, int x, int y) override;
    std::shared_ptr<Gold> createGoldAt(int goldType, int x, int y, std::shared_ptr<Dragon> dragon) override;

    ~SpawnCreator();
};



#endif /* FACTORIES_SPAWNCREATOR_H_ */
