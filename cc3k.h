#ifndef CC3K_H
#define CC3K_H
#include "floor.h"
#include "player/player.h"
#include "potion/potion.h"
#include "stairway.h"
#include <vector>
#include <string>
#include <memory>
class CC3K {
    int levelNum;
    Floor theFloor;

    std::shared_ptr<Stairway> theStairway;

    std::shared_ptr<Player> thePlayer;

    std::vector<std::shared_ptr<Potion>> thePotions;

    int playerGold;

    // Number of potions spawned on each floor
    static const int NUM_POTIONS = 10;

    // Number of gold piles spawned on each floor
    static const int NUM_GOLD = 10;

    // A 2D char vector for displaying
    std::vector<std::vector<char>> theDisplay;

    bool isOccupied(int x, int y);
    void generatePlayer(int targetChamberNum);
    void generateStairway(int playerChamberNum);
    void generatePotions();


    public:
        void init();
        void display();
        void movePlayer(std::string dir);
        CC3K();
};
#endif
