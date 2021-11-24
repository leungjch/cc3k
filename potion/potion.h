#ifndef POTION_H
#define POTION_H
#include "../item.h"
#include <string>

class Potion : public Item {
    int deltaHp;
    int deltaAtk;
    int deltaDef;
    bool isPermanent;
    public:
    Potion(std::string name, int deltaHp, int deltaAtk, int deltaDef, bool isPermanent);
};

#endif
