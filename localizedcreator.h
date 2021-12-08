#ifndef LOCALIZEDCREATOR_H_
#define LOCALIZEDCREATOR_H_

#include <memory>
#include "creator.h"

class Potion;
class Gold;
class Dragon;

class LocalizedCreator: public Creator {
public:
	LocalizedCreator(std::shared_ptr<Floor> floorMap);
    virtual std::shared_ptr<Potion> createPotionAt(int potionType, int x, int y) = 0;
    virtual std::shared_ptr<Gold> createGoldAt(int goldType, int x, int y, std::shared_ptr<Dragon> dragon) = 0;
    // add remaining ones

    virtual ~LocalizedCreator() = 0;
};

#endif /* LOCALIZEDCREATOR_H_ */
