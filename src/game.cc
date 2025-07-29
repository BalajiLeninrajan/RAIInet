#include "game.h"
#include <stdexcept>
#include "factories.h"
#include "player.h"
#include "board.h"
#include "cell.h"

void Game::startGame(int nPlayers, const std::vector<std::string> &abilities, const std::vector<std::vector<std::string>> &linkPlacements) {
    // create board
    board = std::make_unique<Board>(8, 8);

    // create player objects
    if (abilities.size() != nPlayers || linkPlacements.size() != nPlayers) {
        throw std::invalid_argument("Incorrect number of abilities/link placements.");
    }
    players.clear();

    for (int i=0; i<nPlayers; ++i) {
        std::vector<std::unique_ptr<Ability>> p_abilities;
        for (auto ch: abilities[i]) {
            p_abilities.push_back(AbilityFactory::createPlayerAbility(ch));
        }
        players.push_back(std::make_unique<Player>(std::move(p_abilities)));
    }

    linkManager = std::make_shared<LinkManager>();
    for (int i=0; i<nPlayers; ++i) {
        linkManager->addLinksForPlayer(linkPlacements[i], players[i].get(), board.get());
    }
    // create board and cells
    // add board
}

Game::Game() {}
Game::~Game() {}
