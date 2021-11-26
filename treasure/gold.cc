#include "gold.h"
#include <string>

Gold::Gold(std::string name, int value) : Item{name, 'G', Color::GREEN}, value{value} {}

int Gold::getValue() {
    return value;
}
