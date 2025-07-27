#pragma once
#include <memory>

#include "ability.h"

class AbilityFactory {
   public:
    static std::unique_ptr<Ability> createPlayerAbility(char id);
};
