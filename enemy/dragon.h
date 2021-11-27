#ifndef DRAGON_H
#define DRAGON_H
#include "enemy.h"
#include "../treasure/dragonHoard.h"
#include <memory>
class Dragon : public Enemy {
    std::shared_ptr<DragonHoard> theHoard;
    public:
    Dragon(std::shared_ptr<DragonHoard> theHoard);
    int attack(std::shared_ptr<Character> defender) override;
    void move(int dx, int dy) override;
    std::shared_ptr<DragonHoard> getHoard();
};
#endif
