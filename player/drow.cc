#include "drow.h"
#include "../character.h"
#include "player.h"
    #include <iostream>

Drow::Drow() :  Player{150, 25, 15, "Drow"} {}

void Drow::applyPotion(std::shared_ptr<Potion> potion, float magnifier, bool nextLevel)
{
    Player::applyPotion(potion, 1.5, nextLevel);
}
