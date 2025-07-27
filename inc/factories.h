#pragma once
#include <memory>

class Ability;

class AbilityFactory {
   public:
    std::shared_ptr<Ability> createPlayerAbility(int id);
};
