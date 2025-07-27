#pragma once
#include <memory>

#include "ability.h"

class AbilityFactory {
   public:
    std::shared_ptr<Ability> createPlayerAbility(char id);
};
