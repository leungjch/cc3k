#include "poisonHealth.h"
#include "potion.h"

PoisonHealth::PoisonHealth() : Potion{
    "Potion of Poison Health", 
    -10, 
    0, 
    0, 
    true,
    "You've been poisoned and permanently lost 10 HP."}
{}
