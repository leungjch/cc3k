#ifndef MERCHANT_H
#define MERCHANT_H
#include "enemy.h"
#include <memory>
class Merchant : public Enemy {
    public:
    Merchant();
    void attack(std::shared_ptr<Character> defender) override;
};
#endif
