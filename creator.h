#ifndef CREATOR_H_
#define CREATOR_H_

#include <vector>
#include <memory>

class Floor;
class Player;
class Stairway;
class Potion;
class Gold;
class Enemy;
class Dragon;

class Creator {
protected:
    std::shared_ptr<Floor> theFloor;

    int getRandomType(std::vector<int> typeWeightings, int totalWeight);
    std::shared_ptr<Player> createPlayer(int playerType);
    std::shared_ptr<Stairway> createStairway();
    std::shared_ptr<Potion> createPotion(int potionType);
    std::shared_ptr<Gold> createGold(int goldType);
    std::shared_ptr<Enemy> createEnemy(int enemyType, bool hostileMerchants);
public:
	Creator(std::shared_ptr<Floor> floorMap);
    virtual ~Creator() = 0;

};

#endif /* CREATOR_H_ */
