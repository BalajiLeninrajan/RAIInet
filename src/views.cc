#include "views.h"

#include <memory>
#include <string>
#include <vector>

#include "board.h"
#include "game.h"

View::View(const std::unique_ptr<Game> &game) : players() {
    for (unsigned id = 0; id < game->getPlayers().size(); ++id) {
        players.push_back({id++, 5, {0, 0}});
    }
}

View::~View() {}

void View::update(std::pair<int, int> old_coords,
                  std::pair<int, int> new_coords) {}
void View::display() const {}

TextView::TextView(const std::unique_ptr<Game> &game, int playerId)
    : View(game),
      board(
          game->getBoard().getBoard().size(),
          std::vector<std::string>(game->getBoard().getBoard()[0].size(), ".")),
      playerId{playerId} {
    for (auto &player : players) {
        char base = 'a' + (8 * (player.id / 2)) - (32 * (player.id % 2));
        if (playerId != player.id) {
            for (int i = 0; i < 8; ++i) {
                player.links[std::string(1, base + i)] = "??";
            }
            continue;
        }

        for (int i = 0; i < 8; ++i) {
            const auto &link = game->getPlayerLink(playerId, i);
            player.links[std::string(1, base + i)] =
                (link.first == Link::LinkType::DATA ? "D" : "V") +
                std::to_string(link.second);
        }
    }
}

void TextView::update(std::pair<int, int> old_coords,
                      std::pair<int, int> new_coords) {}
void TextView::display() const {}
