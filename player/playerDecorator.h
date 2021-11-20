#include "player.h"
#include "playerComponent.h"
#include <memory>

class PlayerDecorator : public PlayerComponent {
    public:
    std::shared_ptr<PlayerComponent> component;
    PlayerDecorator(std::shared_ptr<PlayerComponent> component);
    ~PlayerDecorator();
};