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

// FirewallAbility

FirewallAbility::FirewallAbility() : Ability("Firewall") {}

void FirewallAbility::use(Game& game, LinkManager& linkManager,
                          const std::vector<std::string>& params) {
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
                                   game.getCurrentPlayer());

    used = true;
}

// DownloadAbility

DownloadAbility::DownloadAbility() : Ability("Download") {}

void DownloadAbility::use(Game& game, LinkManager& linkManager,
                          const std::vector<std::string>& params) {
    if (params.size() != 1) {
        throw std::invalid_argument("Invalid number of parameters");
    }
    char linkId = params[0][0];
    LinkManager::LinkKey key;

    if ('a' <= linkId && linkId < 'a' + 8) {
        if (game.getPlayerIndex(*game.getCurrentPlayer()) == 0) {
            throw std::invalid_argument("You can't download your own link");
        }
        key = {game.getPlayers()[0], static_cast<unsigned int>(linkId - 'a')};
    } else if ('A' <= linkId && linkId < 'A' + 8) {
        if (game.getPlayerIndex(*game.getCurrentPlayer()) == 1) {
            throw std::invalid_argument("You can't download your own link");
        }
        key = {game.getPlayers()[1], static_cast<unsigned int>(linkId - 'A')};
    } else if ('h' <= linkId && linkId < 'h' + 8 &&
               game.getPlayers().size() > 2) {
        if (game.getPlayerIndex(*game.getCurrentPlayer()) == 2) {
            throw std::invalid_argument("You can't download your own link");
        }
        key = {game.getPlayers()[2], static_cast<unsigned int>(linkId - 'h')};
    } else if ('H' <= linkId && linkId < 'H' + 8 &&
               game.getPlayers().size() > 3) {
        if (game.getPlayerIndex(*game.getCurrentPlayer()) == 3) {
            throw std::invalid_argument("You can't download your own link");
        }
        key = {game.getPlayers()[3], static_cast<unsigned int>(linkId - 'H')};
    } else {
        throw std::invalid_argument("Invalid link id");
    }

    // TODO: figure out reveal

    game.getCurrentPlayer()->download(key);

    used = true;
}

// LinkBoostAbility

LinkBoostAbility::LinkBoostAbility() : Ability("LinkBoost") {}

void LinkBoostAbility::use(Game& game, LinkManager& linkManager,
                           const std::vector<std::string>& params) {
    if (params.size() != 1) {
        throw std::invalid_argument("Invalid number of parameters");
    }
    char linkId = params[0][0];
    LinkManager::LinkKey key;

    if ('a' <= linkId && linkId < 'a' + 8) {
        if (game.getPlayerIndex(*game.getCurrentPlayer()) != 0) {
            throw std::invalid_argument("You don't own this link");
        }
        key = {game.getPlayers()[0], static_cast<unsigned int>(linkId - 'a')};
    } else if ('A' <= linkId && linkId < 'A' + 8) {
        if (game.getPlayerIndex(*game.getCurrentPlayer()) != 1) {
            throw std::invalid_argument("You don't own this link");
        }
        key = {game.getPlayers()[1], static_cast<unsigned int>(linkId - 'A')};
    } else if ('h' <= linkId && linkId < 'h' + 8 &&
               game.getPlayers().size() > 2) {
        if (game.getPlayerIndex(*game.getCurrentPlayer()) != 2) {
            throw std::invalid_argument("You don't own this link");
        }
        key = {game.getPlayers()[2], static_cast<unsigned int>(linkId - 'h')};
    } else if ('H' <= linkId && linkId < 'H' + 8 &&
               game.getPlayers().size() > 3) {
        if (game.getPlayerIndex(*game.getCurrentPlayer()) != 3) {
            throw std::invalid_argument("You don't own this link");
        }
        key = {game.getPlayers()[3], static_cast<unsigned int>(linkId - 'H')};
    } else {
        throw std::invalid_argument("Invalid link id");
    }

    std::function<std::unique_ptr<Link>(std::unique_ptr<Link>)> lambda =
        [](std::unique_ptr<Link> p) {
            return std::make_unique<LinkBoostDecorator>(std::move(p));
        };
    linkManager.applyDecorator(key, lambda);

    used = true;
}

// PolarizeAbility

PolarizeAbility::PolarizeAbility() : Ability("Polarize") {}

void PolarizeAbility::use(Game& game, LinkManager& linkManager,
                          const std::vector<std::string>& params) {
    if (params.size() != 1) {
        throw std::invalid_argument("Invalid number of parameters");
    }
    char linkId = params[0][0];
    LinkManager::LinkKey key;

    if ('a' <= linkId && linkId < 'a' + 8) {
        if (game.getPlayerIndex(*game.getCurrentPlayer()) != 0) {
            throw std::invalid_argument("You don't own this link");
        }
        key = {game.getPlayers()[0], static_cast<unsigned int>(linkId - 'a')};
    } else if ('A' <= linkId && linkId < 'A' + 8) {
        if (game.getPlayerIndex(*game.getCurrentPlayer()) != 1) {
            throw std::invalid_argument("You don't own this link");
        }
        key = {game.getPlayers()[1], static_cast<unsigned int>(linkId - 'A')};
    } else if ('h' <= linkId && linkId < 'h' + 8 &&
               game.getPlayers().size() > 2) {
        if (game.getPlayerIndex(*game.getCurrentPlayer()) != 2) {
            throw std::invalid_argument("You don't own this link");
        }
        key = {game.getPlayers()[2], static_cast<unsigned int>(linkId - 'h')};
    } else if ('H' <= linkId && linkId < 'H' + 8 &&
               game.getPlayers().size() > 3) {
        if (game.getPlayerIndex(*game.getCurrentPlayer()) != 3) {
            throw std::invalid_argument("You don't own this link");
        }
        key = {game.getPlayers()[3], static_cast<unsigned int>(linkId - 'H')};
    } else {
        throw std::invalid_argument("Invalid link id");
    }

    std::function<std::unique_ptr<Link>(std::unique_ptr<Link>)> lambda =
        [](std::unique_ptr<Link> p) {
            return std::make_unique<PolarizeDecorator>(std::move(p));
        };
    linkManager.applyDecorator(key, lambda);
    used = true;
}

// ScanAbility

ScanAbility::ScanAbility() : Ability("Scan") {}

void ScanAbility::use(Game& game, LinkManager& linkManager,
                      const std::vector<std::string>& params) {
    if (params.size() != 1) {
        throw std::invalid_argument("Invalid number of parameters");
    }
    char linkId = params[0][0];
    LinkManager::LinkKey key;

    if ('a' <= linkId && linkId < 'a' + 8) {
        if (game.getPlayerIndex(*game.getCurrentPlayer()) == 0) {
            throw std::invalid_argument("You own this link");
        }
        key = {game.getPlayers()[0], static_cast<unsigned int>(linkId - 'a')};
    } else if ('A' <= linkId && linkId < 'A' + 8) {
        if (game.getPlayerIndex(*game.getCurrentPlayer()) == 1) {
            throw std::invalid_argument("You own this link");
        }
        key = {game.getPlayers()[1], static_cast<unsigned int>(linkId - 'A')};
    } else if ('h' <= linkId && linkId < 'h' + 8 &&
               game.getPlayers().size() > 2) {
        if (game.getPlayerIndex(*game.getCurrentPlayer()) == 2) {
            throw std::invalid_argument("You own this link");
        }
        key = {game.getPlayers()[2], static_cast<unsigned int>(linkId - 'h')};
    } else if ('H' <= linkId && linkId < 'H' + 8 &&
               game.getPlayers().size() > 3) {
        if (game.getPlayerIndex(*game.getCurrentPlayer()) == 3) {
            throw std::invalid_argument("You own this link");
        }
        key = {game.getPlayers()[3], static_cast<unsigned int>(linkId - 'H')};
    } else {
        throw std::invalid_argument("Invalid link id");
    }

    std::function<std::unique_ptr<Link>(std::unique_ptr<Link>)> lambda =
        [](std::unique_ptr<Link> p) {
            return std::make_unique<RevealDecorator>(std::move(p));
        };
    linkManager.applyDecorator(key, lambda);
    used = true;
}

// BadConnectionAbility

void BadConnectionAbility::use(Game& game, LinkManager& linkManager,
                               const std::vector<std::string>& params) {
    used = true;
}

// QuantumEntanglement

QuantumEntanglementAbility::QuantumEntanglementAbility()
    : Ability("QuantumEntanglement") {}

void QuantumEntanglementAbility::use(Game& game, LinkManager& linkManager,
                                     const std::vector<std::string>& params) {
    if (params.size() != 2) {
        throw std::invalid_argument("Invalid number of parameters");
    }
    char linkId = params[0][0];
    char partnerId = params[1][0];
    LinkManager::LinkKey link;
    LinkManager::LinkKey partner;

    if ('a' <= linkId && linkId < 'a' + 8 && 'a' <= partnerId &&
        partnerId < 'a' + 8) {
        if (game.getPlayerIndex(*game.getCurrentPlayer()) != 0) {
            throw std::invalid_argument("You dont't own these link");
        }
        link = {game.getPlayers()[0], static_cast<unsigned int>(linkId - 'a')};
        partner = {game.getPlayers()[0],
                   static_cast<unsigned int>(partnerId - 'a')};
    } else if ('A' <= linkId && linkId < 'A' + 8 && 'A' <= partnerId &&
               partnerId < 'A' + 8) {
        if (game.getPlayerIndex(*game.getCurrentPlayer()) != 1) {
            throw std::invalid_argument("You dont't own these link");
        }
        link = {game.getPlayers()[1], static_cast<unsigned int>(linkId - 'A')};
        partner = {game.getPlayers()[1],
                   static_cast<unsigned int>(partnerId - 'A')};
    } else if ('h' <= linkId && linkId < 'h' + 8 && 'h' <= partnerId &&
               partnerId < 'h' + 8 && game.getPlayers().size() > 2) {
        if (game.getPlayerIndex(*game.getCurrentPlayer()) != 2) {
            throw std::invalid_argument("You dont't own these link");
        }
        link = {game.getPlayers()[2], static_cast<unsigned int>(linkId - 'h')};
        partner = {game.getPlayers()[2],
                   static_cast<unsigned int>(partnerId - 'h')};
    } else if ('H' <= linkId && linkId < 'H' + 8 && 'H' <= partnerId &&
               partnerId < 'H' + 8 && game.getPlayers().size() > 3) {
        if (game.getPlayerIndex(*game.getCurrentPlayer()) != 3) {
            throw std::invalid_argument("You dont't own these link");
        }
        link = {game.getPlayers()[3], static_cast<unsigned int>(linkId - 'H')};
        partner = {game.getPlayers()[3],
                   static_cast<unsigned int>(partnerId - 'H')};
    } else {
        throw std::invalid_argument("Invalid link id(s)");
    }

    std::function<std::unique_ptr<Link>(std::unique_ptr<Link>)> lambda =
        [&](std::unique_ptr<Link> p) {
            return std::make_unique<QuantumEntanglementDecorator>(
                std::move(p), &linkManager.getLink(partner));
        };
    linkManager.applyDecorator(link, lambda);
    used = true;
}

// PappleAbility

PappleAbility::PappleAbility() : Ability("Papple") {}

void PappleAbility::use(Game& game, LinkManager& linkManager,
                        const std::vector<std::string>& params) {
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

    used = true;
}
