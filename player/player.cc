#include "player.h"

Player::Player() : Character{100, 10, 10, '@'} {}
Player::Player(int hp, int atk, int def) : Character{hp, atk, def, '@'} {}
