#include "merchant.h"
#include "enemy.h"
#include <iostream>
#include <cmath>
using namespace std;

Merchant::Merchant(bool isHostile) : Enemy{30, 70, 5, isHostile, 'M', "Merchant"} {}

int Merchant::attack(std::shared_ptr<Character> defender) {
    int dmg = 0;
    // Check that the defender is in range
    if (abs(defender->getX() - getX()) <= 1 &&
        abs(defender->getY() - getY()) <= 1)
    {
        // 50% chance of missing
        if (rand() % 2 )
        {
            return 0;
        }
        dmg = (int)(ceil((100.0/(100.0+defender->getDef()))*getAtk()));
    }
    // Decrement the health
    defender->setHP(defender->getHP()-dmg);
    return dmg;
}

