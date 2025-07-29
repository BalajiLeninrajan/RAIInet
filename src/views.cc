#include "views.h"

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "board.h"
#include "cell.h"
#include "game.h"
#include "link.h"
#include "player.h"

View::View(const std::unique_ptr<Game> &game) : players() {
    for (unsigned id = 0; id < game->getPlayers().size(); ++id) {
        players.push_back({id, 5, {0, 0}});
    }
}

View::~View() {}

void View::update(std::pair<int, int> old_coords,
                  std::pair<int, int> new_coords) {}
void View::display() const {}

// player index starts at 0
// even index is lowercase, odd index is uppercase
// if we have more than 2 players, e.g. player 3 (index 2) takes letters
// 'h' to 'o'
// assume we don't get past 'z'/'Z'

// std::string type =
//     playerLinks[i].getType() == Link::LinkType::DATA ? "D"
//                                                      : "V";
// links[std::string(1, base + i)] =
//     type + std::to_string(playerLinks[i].getStrength());
TextView::TextView(const std::unique_ptr<Game> &game, unsigned playerId)
    : View(game),
      board(
          game->getBoard().getBoard().size(),
          std::vector<std::string>(game->getBoard().getBoard()[0].size(), ".")),
      playerId{playerId},
      game{game} {
    for (auto &player : players) {
        // player index starts at 0
        // even index is lowercase, odd index is uppercase
        // if we have more than 2 players, e.g. player 3 (index 2) takes letters
        // 'h' to 'o'
        // assume we don't get past 'z'/'Z'
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
            // std::string type =
            //     playerLinks[i].getType() == Link::LinkType::DATA ? "D"
            //                                                      : "V";
            // links[std::string(1, base + i)] =
            //     type + std::to_string(playerLinks[i].getStrength());
        }
    }
}

// how to best approach this?
// I want to either get the cell or get the thing on top of the cell
// how to update link state? are we handling this in coords as well to check the
// link too?
void TextView::update(std::pair<int, int> old_coords,
                      std::pair<int, int> new_coords) {
    // TODO: fix fetching cell
    // wait if we have an old_coords, is it guaranteed to be empty?
    // oh no how do we tell if it is special cell?
    //
    // BaseCell* old_cell = game->getBoard()->getC(old_coords);
    // BaseCell*
    // std::string top = sdjfksi.cellRepresentation(playerId)
    std::string top;
    if (top == "?") {
        board[old_coords.first][old_coords.second] = ".";
    } else if (top.size() == 1) {
        // find the id
        //
    } else {
        // assumes we have "D1"
    }
}

// should linkKey also take in value to update it with?
// if we use linkKey, we need to figure out how to get actual link
// if we just have int playerId, int linkId, and string value, we can easily
// update
void TextView::update(LinkManager::LinkKey linkKey) {
    int index = game->getPlayerIndex(*linkKey.player);
    std::string value = ;
}

void TextView::printPlayer(PlayerStats player) const {
    std::cout << "Player " << player.id + 1 << ":" << std::endl;
    std::cout << "Downloaded: " << player.score.first << ", "
              << player.score.second << std::endl;
    std::cout << "Abilities: " << player.abilities << std::endl;
    for (auto link : player.links) {
        std::cout << link.first << ": " << link.second;
    }
}
void TextView::display() const {
    // print other players
    for (auto player : players) {
        if (player.id != playerId) {
            printPlayer(player);
        }
    }
    // print the board
    for (auto line : board) {
        for (auto tile : line) {
            std::cout << tile;
        }
        std::cout << std::endl;
    }
    //  assuming players gets each player in proper order
    std::cout << "Player " << playerId + 1 << ":" << std::endl;
    std::cout << "Downloaded: " << players[playerId].score.first << ", "
              << players[playerId].score.second << std::endl;
    std::cout << "Abilities: " << players[playerId].abilities << std::endl;
    // print links
}
