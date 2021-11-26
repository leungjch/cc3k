#include "halfling.h"
#include "enemy.h"
#include <iostream>
#include <cmath>
using namespace std;

Halfling::Halfling() : Enemy{100, 15, 20, true, 'L', "Halfling"} {}

int Halfling::attack(std::shared_ptr<Character> defender) {
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

