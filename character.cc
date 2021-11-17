#include "character.h"
#include "entity.h"


Character::Character() : hp{0}, atk{0}, def{0} {}
Character::Character(int hp, int atk, int def, char symbol) : Entity{symbol}, hp{hp}, atk{atk}, def{def} {}

