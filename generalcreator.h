#ifndef GENERALCREATOR_H_
#define GENERALCREATOR_H_

#include <memory>
#include "creator.h"

class Floor;
class Player;
class Stairway;
class Potion;
class Gold;
class Enemy;
class Dragon;

class GeneralCreator: public Creator {
public:
	GeneralCreator(std::shared_ptr<Floor> floorMap);

    virtual std::shared_ptr<Player> generatePlayer(int startingRace) = 0;
    virtual std::shared_ptr<Stairway> generateStairway(int playerChamberNum) = 0;
    virtual std::shared_ptr<Potion> generatePotion() = 0;
    virtual std::shared_ptr<Gold> generateGold(std::shared_ptr<Dragon> dragonEnemy) = 0;
    virtual std::shared_ptr<Enemy> generateEnemy(bool hostileMerchants) = 0;

    virtual void placePlayer(std::shared_ptr<Player> thePlayer) = 0;

	virtual ~GeneralCreator() = 0;
};

#endif /*GENERALCREATOR_H_ */
