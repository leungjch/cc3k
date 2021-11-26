#include "enemy.h"
#include "../character.h"
#include <string>
#include <memory>
using namespace std;

Enemy::Enemy(int startingHp, int startingAtk, int startingDef, bool hostile, char symbol, std::string name) :
    Character{startingHp, startingAtk, startingDef, symbol, name, Color::RED}, hostile{hostile} {}

int Enemy::attack(shared_ptr<Character> defender) { return 0;}

bool Enemy::inRange(shared_ptr<Character> defender) {
    return (abs(defender->getX() - getX()) <= 1 &&
        abs(defender->getY() - getY()) <= 1);

}

// void Enemy::move(int dx, int dy) {
//     // Discard dx and dy
//     // Generate a random -1, 0, 1
//     int newX = getX() + rand() % 3 - 1;
//     int newY = getY() + rand() % 3 - 1;
//     setX(newX);
//     setY(newY);
// }
