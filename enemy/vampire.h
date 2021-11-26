#ifndef VAMPIRE_H
#define VAMPIRE_H
#include "enemy.h"
#include <memory>
class Vampire : public Enemy {
    public:
    Vampire();
    void attack(std::shared_ptr<Character> defender) override;
};
#endif
