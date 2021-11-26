#ifndef ELF_H
#define ELF_H
#include "enemy.h"
#include <memory>
class Elf : public Enemy {
    public:
    Elf();
    int attack(std::shared_ptr<Character> defender) override;
};
#endif
