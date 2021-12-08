#ifndef LEVELCREATOR_H_
#define LEVELCREATOR_H_

#include <memory>
#include "generalcreator.h"

class CC3K;
class Floor;
class Player;
class Stairway;
class Potion;
class Gold;
class Enemy;
class Dragon;

class LevelCreator: public GeneralCreator {
	const CC3K &game;

public:
	LevelCreator(std::shared_ptr<Floor> floorMap, const CC3K &game);

    std::shared_ptr<Player> generatePlayer(int startingRace) override;
    std::shared_ptr<Stairway> generateStairway(int playerChamberNum) override;
    std::shared_ptr<Potion> generatePotion() override;
    std::shared_ptr<Gold> generateGold(std::shared_ptr<Dragon> dragonEnemy) override;
    std::shared_ptr<Enemy> generateEnemy(bool hostileMerchants) override;
    void placePlayer(std::shared_ptr<Player> thePlayer) override;
};

#endif /* LEVELCREATOR_H_ */
