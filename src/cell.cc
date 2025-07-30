#include "cell.h"

#include <memory>
#include <stdexcept>

#include "game.h"
#include "link.h"
#include "linkmanager.h"
#include "player.h"
#include "views.h"

bool BaseCell::isOccupied() const { return linkKey.has_value(); }

LinkManager::LinkKey BaseCell::getOccupantLink() const {
    if (isOccupied()) return linkKey.value();
    throw std::invalid_argument("Tried to get link from empty cell");
}

void BaseCell::setOccupantLink(LinkManager::LinkKey new_link) {
    if (!isOccupied()) {
        linkKey = new_link;
    }
}

void BaseCell::emptyCell() { linkKey.reset(); }

std::string BaseCell::cellRepresentation(const Game* game) const {
    if (!linkKey.has_value()) {
        return ".";
    }
    int index = game->getPlayerIndex(*linkKey.value().player);
    char link_char = TextView::findBase(index) + linkKey.value().id;
    return std::string(1, link_char);
}

BoardCell::~BoardCell() {}

// onEnter should check for collision and handle it
void BoardCell::onEnter(LinkManager::LinkKey link, Game* game) {
    if (!isOccupied()) {
        setOccupantLink(link);
        return;
    }
    if (link.player == getOccupantLink().player) {
        throw std::invalid_argument("Cannot move onto own link");
    }
    // handles battle, winner downloads loser and loser gets deleted
    if (game->getLinkManager().getLink(link).getStrength() >=
        game->getLinkManager().getLink(getOccupantLink()).getStrength()) {
        link.player->download(getOccupantLink());
        setOccupantLink(link);
        return;
    }
    getOccupantLink().player->download(link);
}

PlayerCell::PlayerCell(std::unique_ptr<BaseCell> base, Player* owner)
    : base{std::move(base)}, owner{owner} {}

void Server::onEnter(LinkManager::LinkKey link, Game* game) {
    if (link.player == owner) {
        throw std::invalid_argument("Cannot move onto own server");
    }
    // download + delete
    // assuming player does not deal with deleting when downloading
    owner->download(link);
}

// Always true to prevent decorating
bool Server::isOccupied() const { return true; }

void Firewall::onEnter(LinkManager::LinkKey link, Game* game) {
    if (link.player != owner) {
        if (!game->getLinkManager().getLink(link).getRevealState()) {
            std::function<std::unique_ptr<Link>(std::unique_ptr<Link>)> fcn =
                [](std::unique_ptr<Link> p) {
                    return std::make_unique<RevealDecorator>(std::move(p));
                };
            game->getLinkManager().applyDecorator(link, fcn);
            std::string type = game->getLinkManager().getLink(link).getType() ==
                                       Link::LinkType::DATA
                                   ? "D"
                                   : "V";
            std::string strength = std::to_string(
                game->getLinkManager().getLink(link).getStrength());
            game->addUpdate(game->getPlayerIndex(*owner), link.id,
                            type + strength);
        }
        if (game->getLinkManager().getLink(link).getType() ==
            Link::LinkType::VIRUS) {
            owner->download(link);
        }
    }
    base->onEnter(link, game);
}

std::string Firewall::cellRepresentation(const Game* game) const {
    if (!isOccupied()) {
        switch (game->getPlayerIndex(*owner)) {
            case 0:
                return "m";
            case 1:
                return "w";
            // TODO: change firewall for player 3 and 4
            case 2:
                return "3";
            case 3:
                return "5";
            default:
                throw std::invalid_argument("invalid index");
        }
    }
    int index = game->getPlayerIndex(*getOccupantLink().player);
    char link_char = TextView::findBase(index) + getOccupantLink().id;
    return std::string(1, link_char);
}

void Goal::onEnter(LinkManager::LinkKey link, Game* game) {
    if (link.player == owner) {
        throw std::invalid_argument("Cannot move onto own goal");
    }
    link.player->download(link);
}
