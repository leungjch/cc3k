#ifndef DROW_H
#define DROW_H

#include "player.h"

class Drow : public Player {
    public:
        Drow();
        void applyPotion(std::shared_ptr<Potion> potion, float magnifier = 1.0, bool nextLevel = false) override;

};
#endif
