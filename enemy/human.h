#ifndef HUMAN_H
#define HUMAN_H
#include "enemy.h"
#include <memory>
class Human : public Enemy {
    public:
    Human();
    void attack(std::shared_ptr<Character> defender) override;
};
#endif
