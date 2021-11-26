#include "elf.h"
#include "enemy.h"
#include <iostream>
#include <cmath>
using namespace std;

Elf::Elf() : Enemy{140, 30, 10, true, 'E', "Elf"} {}

int Elf::attack(std::shared_ptr<Character> defender) {
    int dmg = 0;
    // Check that the defender is in range
    if (abs(defender->getX() - getX()) <= 1 &&
        abs(defender->getY() - getY()) <= 1)
    {
        dmg = (int)(ceil((100.0/(100.0+defender->getDef()))*getAtk()));
    }
    // Decrement the health
    defender->setHP(defender->getHP()-dmg);
    return dmg;
}

