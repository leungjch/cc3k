#include "player.h"
#include "../character.h"
#include <string>
#include <cmath>
#include <vector>
#include "../potion/potion.h"
#include <memory>
using namespace std;

Player::Player(int hp, int atk, int def, string race) : Character{hp, atk, def, '@', race},
                                                        startingHp{hp}, startingAtk{atk}, startingDef{def}
{
}

void Player::applyPotion(shared_ptr<Potion> potion, float magnifier, bool newLevel)
{
    if (!newLevel)
    {
        // Store the potion
        potionsConsumed.push_back(potion);
    }

    // Apply the potion effect with magnifier (default is 1.0x, 1.5x for drow)
    // Stats cannot drop below zero
    setHP(max(0, (int)((float)hp + (float)(magnifier*potion->getDeltaHp()))));
    setAtk(max(0, (int)((float)atk + (float)(magnifier*potion->getDeltaAtk()))));
    setDef(max(0, (int)((float)def + (float)(magnifier*potion->getDeltaDef()))));

    // Max HP for all races is the starting HP, except for vampires that have no maximum.
    // Cap the health to startingHp if it exceeds it
    if (getName() != "Vampire")
    {
        setHP(min(startingHp, hp));
    }
}

void Player::applyPermanentPotions()
{
    // Reset the starting atk and def
    setAtk(startingAtk);
    setDef(startingDef);
    for (int i = 0; i < potionsConsumed.size(); i++)
    {
        // If it's a temporary potion, remove it
        if (!potionsConsumed[i]->getPermanent())
        {
            potionsConsumed[i] = potionsConsumed.back();
            potionsConsumed.pop_back();
        }
        // Otherwise apply the permanent potion effect
        else
        {
            applyPotion(potionsConsumed[i], true);
        }
    }
}

int Player::attack(std::shared_ptr<Character> defender)
{
    // If attempting to attack a halfing, there is a 50% chance of missing
    if (defender->getName() == "Halfling")
    {
        if (rand() % 2)
        {
            return 0;
        }
    }

    int dmg = 0;
    // Check that the defender is in range
    if (abs(defender->getX() - getX()) <= 1 &&
        abs(defender->getY() - getY()) <= 1)
    {
        dmg = (int)(ceil((100.0 / (100.0 + defender->getDef())) * getAtk()));
    }
    // Decrement the health
    defender->setHP(defender->getHP() - dmg);
    return dmg;
}

// A passive ability is an ability that is applied at every turn
void Player::abilityPassive()
{
    // By default, players have no passive ability
    return;
}
