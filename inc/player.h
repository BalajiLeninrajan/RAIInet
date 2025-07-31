#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "linkmanager.h"

// Forward declarations to avoid circular includes
class Ability;

// Represents a player in the game.
class Player {
    std::pair<int, int> score;  // {data, virus}
    std::vector<std::unique_ptr<Ability>> abilities;
    int abilitiesUsed;
    std::shared_ptr<LinkManager> linkManager;

   public:
    Player(std::vector<std::unique_ptr<Ability>> abilities,
           std::shared_ptr<LinkManager> lm);

    std::pair<int, int> getScore() const;
    void setScore(std::pair<int, int> newScore);

    int getAbilitiesUsed() const;
    void incrementAbilityUse();

    const std::vector<std::unique_ptr<Ability>> &getAbilities() const;

    void download(LinkManager::LinkKey linkKey);
};
