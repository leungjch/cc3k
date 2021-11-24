#include "item.h"
#include "entity.h"
#include <string>
using namespace std; 
Item::Item(string name, char symbol) : Entity{symbol}, name{name} {}

string Item::getName() {
    return name;
}
