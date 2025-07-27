#pragma once
#include <memory>

class Ability;

class AbilityFactory {
   public:
    static std::unique_ptr<Ability> createPlayerAbility(char id);
};
