#include "playerComponent.h"
#ifndef PLAYER_H
#define PLAYER_H
#include <string>
class Player : public PlayerComponent {
  public:
    Player();
    Player(int hp, int atk, int def, std::string race);
};
#endif
