#include "playerComponent.h"
#include "../character.h"
#include <string>
using namespace std; 

PlayerComponent::PlayerComponent() : Character{100, 10, 10, '@', " "} {}
PlayerComponent::PlayerComponent(int hp, int atk, int def, string race) : Character{hp, atk, def, '@', race} {}
