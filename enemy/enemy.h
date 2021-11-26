#ifndef ENEMY_H
#define ENEMY_H
#include "../character.h"
#include <string>
class Enemy : public Character {
    bool hostile;
    bool dropsGold;
    public:
        Enemy(int startingHp, int startingAtk, int startingDef, bool hostile, char symbol, std::string name);
        virtual int attack(std::shared_ptr<Character> defender);
        virtual bool inRange(std::shared_ptr<Character> defender);
        // virtual void move(int dx, int dy);
};
#endif
