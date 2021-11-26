#include "entity.h"
#include "utils/color.h"
#include <string>

using namespace std; 

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
string Entity::getColor() {
    return color;
}
Entity::Entity(int x, int y, char sym) : x{x}, y{y}, symbol{sym}, color{Color::RESET} {}
Entity::Entity(char sym) : symbol{sym}, color{Color::RESET} {}
Entity::Entity(char sym, std::string color) : symbol{sym}, color{color} {}


Entity::Entity() : symbol{' '} {}
