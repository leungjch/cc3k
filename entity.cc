#include "entity.h"

int Entity::getX() {
    return x;
}

int Entity::getY() {
    return y;
}

void Entity::setX(int newX) {
    x = newX;
}
void Entity::setY(int newY) {
    y = newY;
}
char Entity::getSymbol() {
    return symbol;
}
Entity::Entity(int x, int y, char sym) : x{x}, y{y}, symbol{sym} {}
Entity::Entity(char sym) : symbol{sym} {}
Entity::Entity() : symbol{' '} {}
