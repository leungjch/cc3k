#include "../character.h"
#include "../potion/potion.h"
#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <memory>
#include <vector>
class Player : public Character {
  std::vector<std::shared_ptr<Potion>> potionsConsumed;
  // Note: startingHp and Character::hp are different
  //       startingHp is used to track the maximum hp
  //       hp is used to keep track of CURRENT hp
  //       same rule for startingDef/Atk
  int startingHp;
  int startingDef;
  int startingAtk;
  public:
    Player(int hp, int atk, int def, std::string race);
    void applyPotion(std::shared_ptr<Potion> potion);
    void applyPermanentPotions();
    std::string getRace();
    virtual int attack(std::shared_ptr<Character> defender);


    enum RaceTypes {
      SHADE = 1,
      DROW, 
      VAMPIRE,
      TROLL,
      GOBLIN
    };

};
#endif
