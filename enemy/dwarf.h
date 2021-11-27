#ifndef DWARF_H
#define DWARF_H
#include "enemy.h"
#include <memory>
class Dwarf : public Enemy {
    public:
    Dwarf();
    int attack(std::shared_ptr<Character> defender) override;
};
#endif