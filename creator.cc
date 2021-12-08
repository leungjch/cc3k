#include "creator.h"

#include "floor.h"

#include "player/player.h"
#include "player/drow.h"
#include "player/shade.h"
#include "player/goblin.h"
#include "player/troll.h"
#include "player/vampire.h"

#include "enemy/enemy.h"
#include "enemy/orc.h"
#include "enemy/human.h"
#include "enemy/dwarf.h"
#include "enemy/halfling.h"
#include "enemy/elf.h"
#include "enemy/merchant.h"

#include "potion/potion.h"
#include "potion/restoreHealth.h"
#include "potion/poisonHealth.h"
#include "potion/boostAtk.h"
#include "potion/woundAtk.h"
#include "potion/boostDef.h"
#include "potion/woundDef.h"

#include "treasure/gold.h"
#include "treasure/smallPile.h"
#include "treasure/normalPile.h"
#include "treasure/merchantHoard.h"

#include "stairway.h"

using namespace std;


Creator::Creator(shared_ptr<Floor> floorMap): theFloor{floorMap} {}

Creator::~Creator() {};

// requires: sum of numbers in typeWeightings equals totalWeight
int Creator::getRandomType(vector<int> typeWeightings, int totalWeight) {
	int sum = rand() % totalWeight + 1;
	int index = 0;
	while (sum > typeWeightings[index]) {
		sum -= typeWeightings[index];
		index++;
	}

	return index + 1;
}

std::shared_ptr<Player> Creator::createPlayer(int playerType) {
	switch (playerType)
	{
	case (Player::RaceTypes::SHADE):
	{
		return make_shared<Shade>();
	}
	case (Player::RaceTypes::DROW):
	{
		return make_shared<Drow>();
	}
	case (Player::RaceTypes::VAMPIRE):
	{
		return make_shared<Vampire>();
	}
	case (Player::RaceTypes::GOBLIN):
	{
		return make_shared<Goblin>();
	}
	case (Player::RaceTypes::TROLL):
	{
		return make_shared<Troll>();
	}
	}

	return nullptr;
}

std::shared_ptr<Stairway> Creator::createStairway() {
	return make_shared<Stairway>();
}

std::shared_ptr<Potion> Creator::createPotion(int potionType) {
	switch (potionType)
	{
	case Potion::PotionTypes::RESTOREHEALTH:
	{
		return make_shared<RestoreHealth>();
	}
	case Potion::PotionTypes::POISONHEALTH:
	{
		return make_shared<PoisonHealth>();
	}
	case Potion::PotionTypes::BOOSTATK:
	{
		return make_shared<BoostAtk>();
	}
	case Potion::PotionTypes::WOUNDATK:
	{
		return make_shared<WoundAtk>();
	}
	case Potion::PotionTypes::BOOSTDEF:
	{
		return make_shared<BoostDef>();
	}
	case Potion::PotionTypes::WOUNDDEF:
	{
		return make_shared<WoundDef>();
	}
	}

	return nullptr;
}

// dragon and dragon hoard are not included
std::shared_ptr<Gold> Creator::createGold(int goldType) {
	switch (goldType)
	{
	case Gold::GoldTypes::SMALL:
	{
		return make_shared<SmallPile>();
	}
	case Gold::GoldTypes::MEDIUM:
	{
		return make_shared<NormalPile>();
	}
	case Gold::GoldTypes::MERCHANT_HOARD:
	{
		return make_shared<MerchantHoard>();
	}
	}

	return nullptr;
}
std::shared_ptr<Enemy> Creator::createEnemy(int enemyType, bool hostileMerchants) {
	switch (enemyType)
	{
	case Enemy::EnemyTypes::HUMAN:
	{
		return make_shared<Human>();
	}
	case Enemy::EnemyTypes::DWARF:
	{
		return make_shared<Dwarf>();
	}
	case Enemy::EnemyTypes::HALFING:
	{
		return make_shared<Halfling>();
	}
	case Enemy::EnemyTypes::ELF:
	{
		return make_shared<Elf>();
	}
	case Enemy::EnemyTypes::ORC:
	{
		return make_shared<Orc>();
	}
	case Enemy::EnemyTypes::MERCHANT:
	{
		return make_shared<Merchant>(hostileMerchants);
	}
	}

	return nullptr;
}
