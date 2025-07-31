#include "ability.h"

#include <execution>
#include <memory>
#include <stdexcept>
#include <string>

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
        coords.second = std::stoi(params[0]);
        coords.first = std::stoi(params[1]) - 1;
        baseCell =
            std::move(game.getBoard().getBoard()[coords.first][coords.second]);
    } catch (const std::invalid_argument& e) {
        throw std::invalid_argument("Invalid coordinates");
    }

    if (baseCell->isOccupied() || !baseCell->canDecorate()) {
        throw std::invalid_argument("Cell is occupied or is a server");
    }

    game.getBoard().getBoard()[coords.first][coords.second] =
        std::make_unique<Firewall>(std::move(baseCell),
                                   game.getCurrentPlayer());

    View::CellUpdate cellUpdate{coords.first, coords.second};

    unsigned playerId = game.getPlayerIndex(*game.getCurrentPlayer());

    game.getCurrentPlayer()->incrementAbilityUse();

    unsigned abilityCount = game.getCurrentPlayer()->getAbilities().size() -
                            game.getCurrentPlayer()->getAbilitiesUsed();
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
        throw std::invalid_argument("You can't download a link you own");
    }

    const auto& link = game.getLinkManager().getLink(key);

    View::CellUpdate cellUpdate{link.getCoords().first,
                                link.getCoords().second};

    unsigned playerId = game.getPlayerIndex(*game.getCurrentPlayer());

    std::string value = (link.getType() == Link::LinkType::DATA ? "D" : "V") +
                        std::to_string(link.getStrength());
    View::RevealLinkUpdate revealUpdate{playerId, key.id, value};

    game.getCurrentPlayer()->download(key);

    View::ScoreUpdate scoreUpdate{playerId,
                                  game.getCurrentPlayer()->getScore()};

    game.getCurrentPlayer()->incrementAbilityUse();

    unsigned abilityCount = game.getCurrentPlayer()->getAbilities().size() -
                            game.getCurrentPlayer()->getAbilitiesUsed();
    View::AbilityCountUpdate abilityCountUpdate{playerId, abilityCount};

    game.addUpdate(cellUpdate);
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

    game.getCurrentPlayer()->incrementAbilityUse();

    unsigned abilityCount = game.getCurrentPlayer()->getAbilities().size() -
                            game.getCurrentPlayer()->getAbilitiesUsed();
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
                        std::to_string(link.getStrength());
    View::RevealLinkUpdate revealUpdate{playerId, key.id, value};

    game.getCurrentPlayer()->incrementAbilityUse();

    unsigned abilityCount = game.getCurrentPlayer()->getAbilities().size() -
                            game.getCurrentPlayer()->getAbilitiesUsed();
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

    unsigned playerId = game.getPlayerIndex(*game.getCurrentPlayer());

    std::string value = (link.getType() == Link::LinkType::DATA ? "D" : "V") +
                        std::to_string(link.getStrength());
    unsigned oppId = game.getPlayerIndex(*key.player);

    View::RevealLinkUpdate revealUpdate{oppId, key.id, value};

    game.getCurrentPlayer()->incrementAbilityUse();

    unsigned abilityCount = game.getCurrentPlayer()->getAbilities().size() -
                            game.getCurrentPlayer()->getAbilitiesUsed();
    View::AbilityCountUpdate abilityCountUpdate{playerId, abilityCount};

    game.addUpdate(revealUpdate);
    game.addUpdate(abilityCountUpdate);
    used = true;
}

// WormHole
WormHoleAbility::WormHoleAbility() : Ability("WormHole") {}

void WormHoleAbility::use(Game& game, const std::vector<std::string>& params) {
    if (params.size() != 2) {
        throw std::invalid_argument("Invalid number of parameters");
    }
    char linkId = params[0][0];
    char partnerId = params[1][0];
    auto linkKey = Ability::getLinkKeyFromId(game, linkId);
    auto partnerKey = Ability::getLinkKeyFromId(game, partnerId);
    if (partnerKey.player != linkKey.player &&
        linkKey.player != game.getCurrentPlayer()) {
        throw std::invalid_argument("You must own both links to swap them");
    }

    auto& link = game.getLinkManager().getLink(linkKey);
    auto& partner = game.getLinkManager().getLink(partnerKey);

    auto linkCoords = link.getCoords();
    auto partnerCoords = partner.getCoords();

    game.getBoard().getCell(linkCoords).setOccupantLink(partnerKey);
    game.getBoard().getCell(partnerCoords).setOccupantLink(linkKey);

    link.setCoords(partnerCoords);
    partner.setCoords(linkCoords);

    game.getCurrentPlayer()->incrementAbilityUse();

    View::CellUpdate linkCellUpdate{linkCoords.first, linkCoords.second};
    View::CellUpdate partnerCellUpdate{partnerCoords.first,
                                       partnerCoords.second};

    unsigned abilityCount = game.getCurrentPlayer()->getAbilities().size() -
                            game.getCurrentPlayer()->getAbilitiesUsed();
    View::AbilityCountUpdate abilityCountUpdate{
        game.getPlayerIndex(*game.getCurrentPlayer()), abilityCount};

    game.addUpdate(abilityCountUpdate);
    game.addUpdate(linkCellUpdate);
    game.addUpdate(partnerCellUpdate);
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

    game.getCurrentPlayer()->incrementAbilityUse();

    unsigned abilityCount = game.getCurrentPlayer()->getAbilities().size() -
                            game.getCurrentPlayer()->getAbilitiesUsed();
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
    std::vector<std::pair<int, int>> corners = {
        {1, 0},
        {board.size() - 2, 0},
        {board.size() - 2, board[0].size() - 1},
        {1, board[0].size() - 1}};
    for (std::pair<int, int> corner : corners) {
        BaseCell& cell = game.getBoard().getCell(corner);
        if (!cell.isOccupied() ||
            cell.getOccupantLink().player != currentPlayer) {
            throw std::runtime_error(
                "YOU ARE NOT WORTHY OF THE POWA OF PAPPLE");
        }
    }

    game.getCurrentPlayer()->setScore({69, 0});

    unsigned playerId = game.getPlayerIndex(*game.getCurrentPlayer());

    View::ScoreUpdate scoreUpdate{playerId,
                                  game.getCurrentPlayer()->getScore()};

    game.getCurrentPlayer()->incrementAbilityUse();

    unsigned abilityCount = game.getCurrentPlayer()->getAbilities().size() -
                            game.getCurrentPlayer()->getAbilitiesUsed();
    View::AbilityCountUpdate abilityCountUpdate{playerId, abilityCount};

    game.addUpdate(scoreUpdate);
    game.addUpdate(abilityCountUpdate);

    used = true;
}
