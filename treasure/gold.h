#ifndef GOLD_H
#define GOLD_H
#include "../item.h"
#include <string>
class Gold : public Item {
    int value;
    public:
    Gold(std::string name, int value);
    int getValue();

    const static int NUM_GOLD_TYPES = 4;

    enum GoldTypes {
        SMALL = 1,
        MEDIUM,
        MERCHANT_HOARD,
        DRAGON_HOARD,
    };
};
#endif
