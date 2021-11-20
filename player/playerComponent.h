#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H
#include "../character.h"
#include <string>

class PlayerComponent : public Character{
    public:
    PlayerComponent();
    PlayerComponent(int hp, int atk, int def, std::string race);
};

#endif