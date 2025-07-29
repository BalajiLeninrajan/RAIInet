#include "game.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

#include "ability.h"
#include "board.h"
#include "cell.h"
#include "factories.h"
#include "link.h"
#include "player.h"

using LinkKey = LinkManager::LinkKey;

void Game::startGame(
    int nPlayers, const std::vector<std::string>& abilities,
    const std::vector<std::vector<std::string>>& linkPlacements) {
    linkManager = std::make_shared<LinkManager>();
    // create board
    board = std::make_unique<Board>(8, 8, linkManager);

    // create player objects
    if (abilities.size() != nPlayers || linkPlacements.size() != nPlayers) {
        throw std::invalid_argument(
            "Incorrect number of abilities/link placements.");
    }

    players.clear();

    for (int i = 0; i < nPlayers; ++i) {
        std::vector<std::unique_ptr<Ability>> p_abilities;
        for (auto ch : abilities[i]) {
            p_abilities.push_back(AbilityFactory::createPlayerAbility(ch));
        }
        players.push_back(
            std::make_unique<Player>(std::move(p_abilities), linkManager));
        linkManager->addLinksForPlayer(linkPlacements[i], players[i].get(),
                                       board.get());
    }

    for (int i = 0; i < nPlayers; ++i) {
        linkManager->addLinksForPlayer(linkPlacements[i], players[i].get(),
                                       board.get());
    }
    currentPlayerIndex = 0;
}

Player* Game::checkWinLoss() {
    // count players
    int activePlayerCount = 0;
    Player* p = nullptr;
    for (const auto& it : players) {
        if (it != nullptr) {
            activePlayerCount++;
            p = it.get();
            // downloaded 4 links
            // implicit assertion: only 1 player can reach 4 links before this
            // is called (does not handle winning ties)
            if (p->getScore().first >= 4) {
                return p;
            }
        }
    }

    if (activePlayerCount == 1) {
        return p;
    }
    return nullptr;
}

int Game::getPlayerIndex(const Player& player) const {
    for (int i = 0; i < players.size(); ++i) {
        if (players[i].get() == &player) return i;
    }
    return -1;
}

void Game::nextTurn() {
    do {
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    } while (players[currentPlayerIndex] != nullptr);
}

void Game::makeMove(int link, char dir) {
    LinkKey linkKey = LinkKey{players[currentPlayerIndex].get(), link};
    linkManager->getLink(linkKey).requestMove(Link::charToDirection(dir));
    nextTurn();
}

void Game::useAbility(int id, const std::vector<std::string>& params) {
    std::cout << "I'm using an ability RAAAHHHHHHH" << "\n";
}

std::vector<Player*> Game::getPlayers() {
    std::vector<Player*> result(players.size());
    std::transform(players.begin(), players.end(), result.begin(),
                   [](const std::unique_ptr<Player>& p) { return p.get(); });
    return result;
}

Board& Game::getBoard() { return *board; }

const std::pair<Link::LinkType, int> Game::getPlayerLink(
    const int playerId, const int linkId) const {
    LinkKey linkKey = LinkKey{players[playerId].get(), linkId};
    const Link& link = linkManager->getLink(linkKey);
    return {link.getType(), link.getStrength()};
}

Game::Game() {}

Game::~Game() {}
