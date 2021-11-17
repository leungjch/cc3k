#ifndef CHARACTER_H
#define CHARACTER_H

#include "entity.h"

class Character : public Entity {
    private:
        int hp;
        int atk;
        int def;
    public:
        // virtual void move(int x, int y);
        Character();
        Character(int hp, int atk, int def, char symbol);
        void move(int dx, int dy);
};
#endif
