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
  protected:
  int startingHp;
  int startingDef;
  int startingAtk;
  float potionMagnifier;
  public:
    Player(int hp, int atk, int def, std::string race);
    Player(int hp, int atk, int def, std::string race, float potionMagnifier);
    virtual void applyPotion(std::shared_ptr<Potion> potion, bool nextLevel = false);
    void applyPermanentPotions();
    virtual int attack(std::shared_ptr<Character> defender);
    virtual void abilityPassive();


    enum RaceTypes {
      SHADE = 1,
      DROW, 
      VAMPIRE,
      TROLL,
      GOBLIN
    };

};
#endif
