#include "character.h"
#include "entity.h"

#include <string>

using namespace std;


Character::Character() : hp{0}, atk{0}, def{0} {}
Character::Character(int hp, int atk, int def, char symbol, string name) : Entity{symbol}, hp{hp}, atk{atk}, def{def}, name{name} {}

// Move the character by a
// Left: dx = -1, dy = 0 
// Right: dx = 1, dy = 0
// Up: dx = 0, dy = -1
// Down: dx = 0, dy = 1
void Character::move(int dx, int dy) {
    setX(getX()+dx);
    setY(getY()+dy);
}

int Character::getHP() {
    return hp;
}

int Character::getAtk() {
    return atk;
}

int Character::getDef() {
    return def;
}

string Character::getName() {
    return name;
}
