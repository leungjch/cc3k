#include "potion.h"
#include "../item.h"
#include <string>
using namespace std;

Potion::Potion(string name, int deltaHp, int deltaAtk, int deltaDef, bool isPermanent) : Item{name, 'P'}, deltaHp{deltaHp}, deltaAtk{deltaAtk}, deltaDef{deltaDef}, isPermanent{isPermanent} {}
