#ifndef CC3K_H
#define CC3K_H
#include "floor.h"
#include "player/player.h"
#include "potion/potion.h"
#include "stairway.h"
#include "treasure/gold.h"
#include <vector>
#include <string>
#include <memory>
#include "display/observer.h"
#include "display/subject.h"
class CC3K : public Subject {
    int levelNum;
    std::shared_ptr<Floor> theFloor;
    std::vector<std::shared_ptr<Observer>> observers;

    std::shared_ptr<Stairway> theStairway;

    std::shared_ptr<Player> thePlayer;

    // Vector of all potions on the floor
    std::vector<std::shared_ptr<Potion>> thePotions;

    // Vector of all gold piles on floor
    std::vector<std::shared_ptr<Gold>> theGold;

    // Starting race (value is specified via Player::RaceTypes enum)
    int startingRace;

    // Amount of gold that the player has collected
    int playerGold;

    // Number of potions spawned on each floor
    static const int NUM_POTIONS = 10;

    // Number of gold piles spawned on each floor
    static const int NUM_GOLD = 10;

    // Number of enemies spawned on each floor
    static const int NUM_ENEMIES = 20;

    // A 2D char vector for displaying the board
    std::vector<std::vector<char>> theDisplay;

    bool isOccupied(int x, int y);
    
    void generatePlayer();
    void placePlayer();
    void generateStairway();
    void generatePotions();
    void generateGold();

    public:
        void newGame();
        void newLevel();
        void display();
        void render();
        void movePlayer(std::string dir);
        void usePotion(std::string dir);
        CC3K();
        void setStartingRace(int newRace);
        char getState(int x, int y) override;
        std::string getGameStatus();
};
#endif
