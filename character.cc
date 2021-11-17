#include "character.h"
#include "entity.h"


Character::Character() : hp{0}, atk{0}, def{0} {}
Character::Character(int hp, int atk, int def, char symbol) : Entity{symbol}, hp{hp}, atk{atk}, def{def} {}

// Move the character by a
// Left: dx = -1, dy = 0 
// Right: dx = 1, dy = 0
// Up: dx = 0, dy = -1
// Down: dx = 0, dy = 1
void Character::move(int dx, int dy) {
    setX(getX()+dx);
    setY(getY()+dy);
}
