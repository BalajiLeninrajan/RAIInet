#pragma once
#include <memory>

#include "ability.h"
#include "player.h"

class AbilityFactory {
   public:
    std::shared_ptr<Ability> createPlayerAbility(int id);
};
