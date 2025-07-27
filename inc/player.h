#pragma once

#include <memory>
#include <utility>
#include <vector>
#include "ability.h"
#include "link.h"

// Forward declarations to avoid circular includes

// Represents a player in the game.
class Player {
    std::pair<int, int> score;
    std::vector<std::unique_ptr<Ability>> abilities;
    std::vector<std::shared_ptr<Link>> links;

   public:
    Player() {}

    std::pair<int, int> getScore() const;
    const std::vector<std::unique_ptr<Ability>> &getAbilities() const;
    void assignAbility(std::unique_ptr<Ability> ability) {abilities.push_back(std::move(ability));}

    void download(Link &link);
    void deleteLink(Link &link);
    std::weak_ptr<Link> getLink(char linkId);
};
