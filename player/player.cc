#include "player.h"
#include "playerComponent.h"
#include <string>
using namespace std;

Player::Player() : PlayerComponent{100, 10, 10, " "} {}
Player::Player(int hp, int atk, int def, string race) : PlayerComponent{hp, atk, def, race} {}
