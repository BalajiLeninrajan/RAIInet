#include "ability.h"

#include <memory>
#include <stdexcept>

#include "board.h"
#include "cell.h"
#include "game.h"
#include "link.h"
#include "linkmanager.h"
#include "player.h"

Ability::Ability(std::string name) : name(name), used(false) {}

bool Ability::isUsed() const { return used; }

std::string Ability::getName() const { return name; }

LinkManager::LinkKey Ability::getLinkKeyFromId(const Game& game,
                                               const char& linkId) {
    if ('a' <= linkId && linkId < 'a' + 8) {
        return {game.getPlayers()[0], static_cast<unsigned int>(linkId - 'a')};
    } else if ('A' <= linkId && linkId < 'A' + 8) {
        return {game.getPlayers()[1], static_cast<unsigned int>(linkId - 'A')};
    } else if ('h' <= linkId && linkId < 'h' + 8) {
        return {game.getPlayers()[2], static_cast<unsigned int>(linkId - 'h')};
    } else if ('H' <= linkId && linkId < 'H' + 8) {
        return {game.getPlayers()[3], static_cast<unsigned int>(linkId - 'H')};
    } else {
        throw std::invalid_argument("Invalid link id");
    }
}

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
        coords.second = std::stoi(params[1]) - 1;
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
                                   game.getCurrentPlayer());

    View::CellUpdate cellUpdate{coords.first, coords.second};

    unsigned playerId = game.getPlayerIndex(*game.getCurrentPlayer());
    unsigned abilityCount = game.getCurrentPlayer()->getAbilities().size();
    View::AbilityCountUpdate abilityCountUpdate{playerId, abilityCount};

    game.addUpdate(cellUpdate);
    game.addUpdate(abilityCountUpdate);

    used = true;
}

// DownloadAbility

DownloadAbility::DownloadAbility() : Ability("Download") {}

void DownloadAbility::use(Game& game, const std::vector<std::string>& params) {
    if (params.size() != 1) {
        throw std::invalid_argument("Invalid number of parameters");
    }
    char linkId = params[0][0];
    auto key = Ability::getLinkKeyFromId(game, linkId);
    if (key.player == game.getCurrentPlayer()) {
        throw std::invalid_argument("You can't downlaod a link you own");
    }

    game.getCurrentPlayer()->download(key);

    const auto& link = game.getLinkManager().getLink(key);

    unsigned playerId = game.getPlayerIndex(*game.getCurrentPlayer());

    std::string value = (link.getType() == Link::LinkType::DATA ? "D" : "V") +
                        link.getStrength();
    View::RevealLinkUpdate revealUpdate{playerId, key.id, value};

    View::ScoreUpdate scoreUpdate{playerId, key.player->getScore()};

    unsigned abilityCount = game.getCurrentPlayer()->getAbilities().size();
    View::AbilityCountUpdate abilityCountUpdate{playerId, abilityCount};

    game.addUpdate(scoreUpdate);
    game.addUpdate(abilityCountUpdate);

    used = true;
}

// LinkBoostAbility

LinkBoostAbility::LinkBoostAbility() : Ability("LinkBoost") {}

void LinkBoostAbility::use(Game& game, const std::vector<std::string>& params) {
    if (params.size() != 1) {
        throw std::invalid_argument("Invalid number of parameters");
    }
    char linkId = params[0][0];
    auto key = Ability::getLinkKeyFromId(game, linkId);
    if (key.player != game.getCurrentPlayer()) {
        throw std::invalid_argument("You can only boost links you own");
    }

    std::function<std::unique_ptr<Link>(std::unique_ptr<Link>)> lambda =
        [](std::unique_ptr<Link> p) {
            return std::make_unique<LinkBoostDecorator>(std::move(p));
        };
    game.getLinkManager().applyDecorator(key, lambda);

    unsigned playerId = game.getPlayerIndex(*game.getCurrentPlayer());

    unsigned abilityCount = game.getCurrentPlayer()->getAbilities().size();
    View::AbilityCountUpdate abilityCountUpdate{playerId, abilityCount};

    game.addUpdate(abilityCountUpdate);

    used = true;
}

// PolarizeAbility

PolarizeAbility::PolarizeAbility() : Ability("Polarize") {}

void PolarizeAbility::use(Game& game, const std::vector<std::string>& params) {
    if (params.size() != 1) {
        throw std::invalid_argument("Invalid number of parameters");
    }
    char linkId = params[0][0];
    auto key = Ability::getLinkKeyFromId(game, linkId);
    if (key.player != game.getCurrentPlayer()) {
        throw std::invalid_argument("You can only polarize links you own");
    }

    std::function<std::unique_ptr<Link>(std::unique_ptr<Link>)> lambda =
        [](std::unique_ptr<Link> p) {
            return std::make_unique<PolarizeDecorator>(std::move(p));
        };
    game.getLinkManager().applyDecorator(key, lambda);

    const auto& link = game.getLinkManager().getLink(key);
    const auto& coords = link.getCoords();

    unsigned playerId = game.getPlayerIndex(*game.getCurrentPlayer());

    View::CellUpdate cellUpdate{coords.first, coords.second};

    std::string value = (link.getType() == Link::LinkType::DATA ? "D" : "V") +
                        link.getStrength();
    View::RevealLinkUpdate revealUpdate{playerId, key.id, value};

    unsigned abilityCount = game.getCurrentPlayer()->getAbilities().size();
    View::AbilityCountUpdate abilityCountUpdate{playerId, abilityCount};

    game.addUpdate(cellUpdate);
    game.addUpdate(revealUpdate);
    game.addUpdate(abilityCountUpdate);
    used = true;
}

// ScanAbility

ScanAbility::ScanAbility() : Ability("Scan") {}

void ScanAbility::use(Game& game, const std::vector<std::string>& params) {
    if (params.size() != 1) {
        throw std::invalid_argument("Invalid number of parameters");
    }
    char linkId = params[0][0];
    auto key = Ability::getLinkKeyFromId(game, linkId);
    if (key.player == game.getCurrentPlayer()) {
        throw std::invalid_argument("Bro, why are you scanning your own links");
    }

    std::function<std::unique_ptr<Link>(std::unique_ptr<Link>)> lambda =
        [](std::unique_ptr<Link> p) {
            return std::make_unique<RevealDecorator>(std::move(p));
        };
    game.getLinkManager().applyDecorator(key, lambda);

    const auto& link = game.getLinkManager().getLink(key);
    const auto& coords = link.getCoords();

    unsigned playerId = game.getPlayerIndex(*game.getCurrentPlayer());

    View::CellUpdate cellUpdate{coords.first, coords.second};

    std::string value = (link.getType() == Link::LinkType::DATA ? "D" : "V") +
                        link.getStrength();
    unsigned oppId = game.getPlayerIndex(*key.player);
    View::RevealLinkUpdate revealUpdate{oppId, key.id, value};

    unsigned abilityCount = game.getCurrentPlayer()->getAbilities().size();
    View::AbilityCountUpdate abilityCountUpdate{playerId, abilityCount};

    game.addUpdate(cellUpdate);
    game.addUpdate(revealUpdate);
    game.addUpdate(abilityCountUpdate);
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
                                     const std::vector<std::string>& params) {
    if (params.size() != 2) {
        throw std::invalid_argument("Invalid number of parameters");
    }
    char linkId = params[0][0];
    char partnerId = params[1][0];
    auto link = Ability::getLinkKeyFromId(game, linkId);
    auto partner = Ability::getLinkKeyFromId(game, partnerId);
    if (partner.player != link.player &&
        link.player != game.getCurrentPlayer()) {
        throw std::invalid_argument("You must own both links to entangle them");
    }

    std::function<std::unique_ptr<Link>(std::unique_ptr<Link>)> lambda =
        [&](std::unique_ptr<Link> p) {
            return std::make_unique<QuantumEntanglementDecorator>(
                std::move(p), &game.getLinkManager().getLink(partner));
        };
    game.getLinkManager().applyDecorator(link, lambda);

    unsigned playerId = game.getPlayerIndex(*game.getCurrentPlayer());

    unsigned abilityCount = game.getCurrentPlayer()->getAbilities().size();
    View::AbilityCountUpdate abilityCountUpdate{playerId, abilityCount};

    game.addUpdate(abilityCountUpdate);
    used = true;
}

// PappleAbility

PappleAbility::PappleAbility() : Ability("Papple") {}

void PappleAbility::use(Game& game, const std::vector<std::string>& params) {
    // TODO: DESHITTIFY
    const auto& board = game.getBoard().getBoard();
    Player* currentPlayer = game.getCurrentPlayer();
    bool topLeft = board[1][1]->getOccupantLink().player == currentPlayer;
    bool bottomLeft =
        board[board.size() - 2][1]->getOccupantLink().player == currentPlayer;
    bool topRight = board[1][board[0].size() - 2]->getOccupantLink().player ==
                    currentPlayer;
    bool bottomRight = board[board.size() - 2][board[0].size() - 2]
                           ->getOccupantLink()
                           .player == currentPlayer;
    if (!(topLeft && bottomLeft && topRight && bottomRight)) {
        throw std::runtime_error("YOU ARE NOT WORTHY OF THE POWA OF PAPPLE");
    }

    game.getCurrentPlayer()->setScore({69, 0});

    unsigned playerId = game.getPlayerIndex(*game.getCurrentPlayer());

    View::ScoreUpdate scoreUpdate{playerId,
                                  game.getCurrentPlayer()->getScore()};

    unsigned abilityCount = game.getCurrentPlayer()->getAbilities().size();
    View::AbilityCountUpdate abilityCountUpdate{playerId, abilityCount};

    game.addUpdate(scoreUpdate);
    game.addUpdate(abilityCountUpdate);

    used = true;
}
