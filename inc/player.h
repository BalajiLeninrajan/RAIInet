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
    std::shared_ptr<LinkManager> linkManager;

   public:
    Player(std::vector<std::unique_ptr<Ability>> abilities);

    std::pair<int, int> getScore() const;
    const std::vector<std::unique_ptr<Ability>> &getAbilities() const;
    void setLinkManager(std::shared_ptr<LinkManager> lm);

    void download(LinkManager::LinkKey linkKey);
};
