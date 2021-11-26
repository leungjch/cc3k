#ifndef DRAGON_H
#define DRAGON_H
#include "enemy.h"
#include <memory>
class Dragon : public Enemy {
    public:
    Dragon();
    int attack(std::shared_ptr<Character> defender) override;
};
#endif
