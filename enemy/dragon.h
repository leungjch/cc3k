#ifndef DRAGON_H
#define DRAGON_H
#include "enemy.h"
#include <memory>
class Dragon : public Enemy {
    public:
    Dragon();
    void attack(std::shared_ptr<Character> defender) override;
};
#endif
