#include "game.h"
#include <stdexcept>
#include "factories.h"

void Game::startGame(int nPlayers, const std::vector<std::string> &abilities, const std::vector<std::vector<std::string>> &linkPlacements) {
    if (abilities.size() != nPlayers || linkPlacements.size() != nPlayers) {
        throw std::invalid_argument("Incorrect number of abilities/link placements.");
    }
}
