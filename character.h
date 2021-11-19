#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>

#include "entity.h"

class Character : public Entity {
    private:
        int hp;
        int atk;
        int def;
        std::string name;
    public:
        // virtual void move(int x, int y);
        Character();
        Character(int hp, int atk, int def, char symbol, std::string name);
        void move(int dx, int dy);
        int getHP();
        int getAtk();
        int getDef();
        std::string getName();
};
#endif
