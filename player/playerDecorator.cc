#include "playerDecorator.h"
#include "playerComponent.h"
#include "player.h"
#include <memory>
using namespace std; 
PlayerDecorator::PlayerDecorator(shared_ptr<PlayerComponent> component) : PlayerComponent{}, component{component} {}
PlayerDecorator::~PlayerDecorator() {}