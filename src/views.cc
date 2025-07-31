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
#include "linkmanager.h"

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
            std::vector<std::string>(game->getBoard().getBoard()[0].size())) {
    for (unsigned i = 0; i < board.size(); ++i) {
        for (unsigned j = 0; j < board[0].size(); ++j) {
            board[i][j] =
                game->getBoard().getBoard()[i][j]->cellRepresentation(game);
        }
    }

    for (auto &player : players) {
        char base = findBase(player.id);
        if (game->getPlayerIndex(*viewer) != player.id) {
            for (int i = 0; i < 8; ++i) {
                player.links[std::string(1, base + i)] = " ?";
            }
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

void TextView::update(View::CellUpdate update) {
    BaseCell &cell = game->getBoard().getCell({update.row, update.col});
    board[update.row][update.col] = cell.cellRepresentation(game);
}

void TextView::update(View::RevealLinkUpdate update) {
    LinkManager::LinkKey key{game->getPlayers()[update.playerId],
                             update.linkId};
    if (key.player != viewer &&
        !game->getLinkManager().getLink(key).getRevealState()) {
        return;
    }
    char base = findBase(update.playerId);
    players[update.playerId].links[std::string(1, base + update.linkId)] =
        update.value;
}

void TextView::update(View::AbilityCountUpdate update) {
    players[update.playerId].abilities = update.abilityCount;
}

void TextView::update(View::ScoreUpdate update) {
    players[update.playerId].score = update.score;
}

void TextView::printPlayer(PlayerStats player) const {
    std::cout << "Player " << player.id + 1 << ":" << std::endl;
    std::cout << "Downloaded: " << player.score.first << ", "
              << player.score.second << std::endl;
    std::cout << "Abilities: " << player.abilities << std::endl;
    for (auto link : player.links) {
        std::cout << link.first << ": " << link.second;
        std::cout << " ";
    }
    std::cout << std::endl;
}

void TextView::display() const {
    // print other players
    for (auto player : players) {
        if (player.id != game->getPlayerIndex(*viewer)) {
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
    printPlayer(players[game->getPlayerIndex(*viewer)]);
}

GraphicsView::GraphicsView(const Game *game, const Player *viewer)
    : View(game, viewer) {};

void GraphicsView::update(View::CellUpdate update) {
    // TODO: implement
}
void GraphicsView::update(View::RevealLinkUpdate update) {
    // TODO: implement
}
void GraphicsView::update(View::AbilityCountUpdate update) {
    // TODO: implement
}

void GraphicsView::update(View::ScoreUpdate update) {
    // TODO: implement
}

void GraphicsView::display() const {
    // Does nothing
}
