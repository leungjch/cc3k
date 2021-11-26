#include "player.h"
#include "../character.h"
#include <string>
#include <vector>
#include "../potion/potion.h"
#include <memory>
using namespace std;

Player::Player(int hp, int atk, int def, string race) : Character{hp, atk, def, '@', race},
                                                        startingHp{hp}, startingAtk{atk}, startingDef{def}
{
}

void Player::applyPotion(shared_ptr<Potion> potion)
{
    // Store the potion
    potionsConsumed.push_back(potion);

    // Apply the potion effect
    // Stats cannot drop below zero
    setHP(max(0, hp + potion->getDeltaHp()));
    setAtk(max(0, atk + potion->getDeltaAtk()));
    setDef(max(0, def + potion->getDeltaDef()));

    // Max HP for all races is the starting HP, except for vampires that have no maximum.
    // Cap the health to startingHp if it exceeds it
    if (getName() != "Vampire")
    {
        setHP(min(startingHp, hp));
    }
}

void Player::applyPermanentPotions()
{
    // Reset the starting stats
    setHP(startingHp);
    setAtk(startingAtk);
    setDef(startingDef);
    for (int i = 0; i < potionsConsumed.size(); i++)
    {
        // If it's a temporary potion, delete it
        if (!potionsConsumed[i]->getPermanent())
        {
            potionsConsumed[i] = potionsConsumed.back();
            potionsConsumed.pop_back();
        }
        // Otherwise apply the permanent potion effect
        else {
            applyPotion(potionsConsumed[i]);
        }
    }
}
