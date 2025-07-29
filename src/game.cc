#include "game.h"

#include <stdexcept>

#include "board.h"
#include "cell.h"
#include "factories.h"
#include "player.h"

void Game::startGame(
    int nPlayers, const std::vector<std::string> &abilities,
    const std::vector<std::vector<std::string>> &linkPlacements) {
    // create board
    board = std::make_unique<Board>(8, 8);

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
        players.push_back(std::make_unique<Player>(std::move(p_abilities)));
    }

    linkManager = std::make_shared<LinkManager>();
    for (int i = 0; i < nPlayers; ++i) {
        linkManager->addLinksForPlayer(linkPlacements[i], players[i].get(),
                                       board.get());
    }
    // create board and cells
    // add board
}

Player* Game::checkWinLoss() {
    // count players
    int activePlayerCount = 0;
    Player* p = nullptr;
    for (const auto &it: players) {
        if (it != nullptr) {
            activePlayerCount++;
            p = it.get();
            // downloaded 4 links
            // implicit assertion: only 1 player can reach 4 links before this is called
            // (does not handle winning ties)
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

int Game::getPlayerIndex(const Player& player) {
    for (int i=0; i<players.size(); ++i) {
        if (players[i].get() == &player) return i;
    }
    return -1;
}

Game::Game() {}
Game::~Game() {}
