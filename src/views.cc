#include "views.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "game.h"
#include "player.h"

View::~View() {}
void View::update(std::pair<int, int> old_coords,
                  std::pair<int, int> new_coords) {}
void View::display() const {}

TextView::TextView(const std::unique_ptr<Game> &game, int playerId)
    : playerId{playerId} {
    for (auto player : game->getPlayers()) {
        // TODO: figure out how to get links
        players.push_back({game->getPlayerIndex(*player),
                           (int)player->getAbilities().size(),
                           player->getScore(),
                           {}});
    }
}
