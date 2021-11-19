#ifndef CC3K_H
#define CC3K_H
#include "floor.h"
#include "player/player.h"
#include "stairway.h"
#include <vector>
#include <string>
#include <memory>
class CC3K {
    int levelNum;
    Floor theFloor;

    Stairway theStairway;

    std::shared_ptr<Player> thePlayer;
    int playerGold;

    // Entities are on top of the environment
    std::vector<std::vector<Entity>> theEntities;

    // A 2D char vector for displaying
    std::vector<std::vector<char>> theDisplay;

    void generatePlayer(int targetChamberNum);
    void generateStairway(int playerChamberNum);

    public:
        void init();
        void display();
        void movePlayer(std::string dir);
        CC3K();
};
#endif
