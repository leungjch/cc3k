#include "orc.h"
#include "enemy.h"
#include <iostream>
#include <cmath>
using namespace std;

Orc::Orc() : Enemy{180, 30, 25, true, 'O', "Orc"} {}

// void Orc::move(int dx, int dy) {
//     // Default movement
//     Enemy::move(0,0);
// }

int Orc::attack(std::shared_ptr<Character> defender) {
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
