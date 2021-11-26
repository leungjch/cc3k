#ifndef HALFLING_H
#define HALFLING_H
#include "enemy.h"
#include <memory>
class Halfling : public Enemy {
    public:
    Halfling();
    void attack(std::shared_ptr<Character> defender) override;
};
#endif
