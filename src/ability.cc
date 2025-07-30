#include "ability.h"

#include <memory>
#include <stdexcept>

#include "board.h"
#include "cell.h"
#include "game.h"

Ability::Ability(std::string name) : name(name), used(false) {}

// FirewallAbility

FirewallAbility::FirewallAbility() : Ability("Firewall") {}

void FirewallAbility::use(Game& game, const std::vector<std::string>& params) {
    std::pair<int, int> coords;
    if (params.size() != 2) {
        throw std::invalid_argument("Invalid number of parameters");
    }

    std::unique_ptr<BaseCell> baseCell;
    try {
        coords.first = std::stoi(params[0]);
        coords.second = std::stoi(params[1]);
        baseCell =
            std::move(game.getBoard().getBoard()[coords.first][coords.second]);
    } catch (const std::invalid_argument& e) {
        throw std::invalid_argument("Invalid coordinates");
    }

    if (baseCell->isOccupied()) {
        throw std::invalid_argument("Cell is occupied or is a server");
    }

    game.getBoard().getBoard()[coords.first][coords.second] =
        std::make_unique<Firewall>(std::move(baseCell),
                                   *game.getCurrentPlayer());

    used = true;
}

// DownloadAbility

DownloadAbility::DownloadAbility() : Ability("Download") {}

void DownloadAbility::use(Game& game, const std::vector<std::string>& params) {
    used = true;
}

// LinkBoostAbility

LinkBoostAbility::LinkBoostAbility() : Ability("LinkBoost") {}

void LinkBoostAbility::use(Game& game, const std::vector<std::string>& params) {
    used = true;
}

// PolarizeAbility

PolarizeAbility::PolarizeAbility() : Ability("Polarize") {}

void PolarizeAbility::use(Game& game, const std::vector<std::string>& params) {
    used = true;
}

// ScanAbility

ScanAbility::ScanAbility() : Ability("Scan") {}

void ScanAbility::use(Game& game, const std::vector<std::string>& params) {
    used = true;
}

// BadConnectionAbility

void BadConnectionAbility::use(Game& game,
                               const std::vector<std::string>& params) {
    used = true;
}

// QuantumEntanglement

QuantumEntanglementAbility::QuantumEntanglementAbility()
    : Ability("QuantumEntanglement") {}

void QuantumEntanglementAbility::use(Game& game,
                                     const std::vector<std::string>& p) {
    used = true;
}

// PappleAbility

PappleAbility::PappleAbility() : Ability("Papple") {}

void PappleAbility::use(Game& game, const std::vector<std::string>& params) {
    used = true;
}
