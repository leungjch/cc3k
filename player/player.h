#include "../character.h"
#ifndef PLAYER_H
#define PLAYER_H
#include <string>
class Player : public Character {
  public:
    Player();
    Player(int hp, int atk, int def);

};
#endif
