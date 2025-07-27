#include "game.h"
#include <stdexcept>
#include "factories.h"
#include "player.h"
#include "board.h"
#include "cell.h"

void Game::startGame(int nPlayers, const std::vector<std::string> &abilities, const std::vector<std::vector<std::string>> &linkPlacements) {
    if (abilities.size() != nPlayers || linkPlacements.size() != nPlayers) {
        throw std::invalid_argument("Incorrect number of abilities/link placements.");
    }
    players.clear();
    for (int i=0; i<nPlayers; ++i) {
        players.push_back(std::make_unique<Player>());
        for (auto ch: abilities[i]) {

            players[i]->assignAbility(AbilityFactory::createPlayerAbility(ch));
        }
    }
    
}

Game::Game() {}
Game::~Game() {}
