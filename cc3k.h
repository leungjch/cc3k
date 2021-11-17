#ifndef CC3K_H
#define CC3K_H
#include "floor.h"
#include "player/player.h"
#include <vector>
#include <string>
#include <memory>
class CC3K {
    Floor theFloor;

    std::shared_ptr<Player> thePlayer;

    // Entities are on top of the environment
    std::vector<std::vector<Entity>> theEntities;

    std::vector<std::vector<char>> theDisplay;

    void generatePlayer();

    public:
        void init();
        void display();
        void movePlayer(std::string dir);
        CC3K();


};
#endif
