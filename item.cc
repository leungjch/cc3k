#include "item.h"
#include "entity.h"
#include <string>
using namespace std; 
Item::Item(std::string name, char symbol) : Entity{symbol}, name{name} {}
