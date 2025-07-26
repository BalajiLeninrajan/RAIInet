#pragma once
#include "ability.h"
#include "player.h"
#include <memory>

class AbilityFactory {

   public:
    std::shared_ptr<Ability> createPlayerAbility(int id);

};
