#include "player.h"
#include <string>
using namespace std;

Player::Player() : Character{100, 10, 10, '@', " "} {}
Player::Player(int hp, int atk, int def, string race) : Character{hp, atk, def, '@', race} {}
