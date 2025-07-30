#include "views.h"

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "board.h"
#include "cell.h"
#include "game.h"
#include "link.h"

View::View(const Game *game, const Player *viewer)
    : players(), viewer(viewer), game(game) {
    for (unsigned id = 0; id < game->getPlayers().size(); ++id) {
        players.push_back({id, 5, {0, 0}});
    }
}

View::~View() {}

void View::display() const {}

char View::findBase(int index) {
    switch (index) {
        case 0:
            return 'a';
        case 1:
            return 'A';
        // case 2 and 3 are not implemented yet
        case 2:
            return 'h';
        case 3:
            return 'H';
        default:
            throw std::invalid_argument("not a valid index");
    }
}

TextView::TextView(const Game *game, const Player *viewer)
    : View(game, viewer),
      board(game->getBoard().getBoard().size(),
            std::vector<std::string>(game->getBoard().getBoard()[0].size(),
                                     ".")) {
    for (auto &player : players) {
        char base = findBase(player.id);
        if (currentPlayer != player.id) {
            for (int i = 0; i < 8; ++i) {
                player.links[std::string(1, base + i)] = "?";
            }
        } else {
            continue;
        }

        for (int i = 0; i < 8; ++i) {
            const auto &link = game->getPlayerLink(player.id, i);
            player.links[std::string(1, base + i)] =
                (link.first == Link::LinkType::DATA ? "D" : "V") +
                std::to_string(link.second);
        }
    }
}

void TextView::update(std::pair<int, int> coords) {
    BaseCell &cell = game->getBoard().getCell(coords);
    board[coords.first][coords.second] = cell.cellRepresentation(game);
}

void TextView::update(int currentPlayer, int linkId, std::string value) {
    char base = findBase(currentPlayer);
    players[currentPlayer].links[std::string(1, base + linkId)] = value;
}

void TextView::printPlayer(PlayerStats player) const {
    std::cout << "Player " << player.id + 1 << ":" << std::endl;
    std::cout << "Downloaded: " << player.score.first << ", "
              << player.score.second << std::endl;
    std::cout << "Abilities: " << player.abilities << std::endl;
    // TODO: make spacing nicer
    for (auto link : player.links) {
        std::cout << link.first << ": " << link.second;
    }
    std::cout << std::endl;
}

void TextView::display() const {
    // print other players
    for (auto player : players) {
        if (player.id != currentPlayer) {
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
    printPlayer(players[currentPlayer]);
}

GraphicsView::GraphicsView(const Game *game, const Player *viewer)
    : View(game, viewer) {};

void GraphicsView::update(std::pair<int, int> coords) {
    // TODO: implement
}
void GraphicsView::update(int playerId, int linkId, std::string value) {
    // TODO: implement
}

void GraphicsView::display() const {
    // Does nothing
}
