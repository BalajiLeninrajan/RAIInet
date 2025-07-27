#pragma once

#include <memory>
#include <utility>
#include <vector>

// Forward declarations to avoid circular includes
class Ability;
class Link;

// Represents a player in the game.
class Player {
    std::pair<int, int> score;  // {data, virus}
    std::vector<std::unique_ptr<Ability>> abilities;
    std::vector<std::unique_ptr<Link>> links;

   public:
    Player(std::vector<std::unique_ptr<Ability>> abilities,
           std::vector<std::unique_ptr<Link>> links);

    std::pair<int, int> getScore() const;
    const std::vector<std::unique_ptr<Ability>> &getAbilities() const;

    void download(Link &link);
    void deleteLink(Link &link);
    std::unique_ptr<Link> &getLink(int linkId);
};
