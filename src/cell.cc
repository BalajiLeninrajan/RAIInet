#include "cell.h"

#include <memory>
#include <stdexcept>

#include "link.h"
#include "linkmanager.h"
#include "player.h"

bool BaseCell::isOccupied() { return linkKey.has_value(); }

LinkManager::LinkKey BaseCell::getOccupantLink() {
    if (isOccupied()) return linkKey.value();
    throw std::invalid_argument("Tried to get link from empty cell");
}

void BaseCell::setOccupantLink(LinkManager::LinkKey new_link) {
    if (!isOccupied()) {
        linkKey = new_link;
    }
}

std::shared_ptr<LinkManager> BaseCell::getLinkManager() { return linkManager; }

void BaseCell::emptyCell() { linkKey.reset(); }

BoardCell::BoardCell(std::shared_ptr<LinkManager> lm) : BaseCell(lm) {}
BoardCell::~BoardCell() {}

// onEnter should check for collision and handle it
void BoardCell::onEnter(LinkManager::LinkKey link) {
    if (!isOccupied()) {
        setOccupantLink(link);
        return;
    }
    if (link.player == getOccupantLink().player) {
        throw std::invalid_argument("Cannot move onto own link");
    }
    // handles battle, winner downloads loser and loser gets deleted
    if (getLinkManager()->getLink(link).getStrength() >=
        getLinkManager()->getLink(getOccupantLink()).getStrength()) {
        link.player->download(getOccupantLink());
        setOccupantLink(link);
        return;
    }
    getOccupantLink().player->download(link);
}

PlayerCell::PlayerCell(std::unique_ptr<BaseCell> base, Player& owner)
    : BaseCell{nullptr}, base{std::move(base)}, owner{&owner} {}

PlayerCell::~PlayerCell() {}

std::shared_ptr<LinkManager> PlayerCell::getLinkManager() {
    return base->getLinkManager();
}
void Server::onEnter(LinkManager::LinkKey link) {
    if (link.player == owner) {
        throw std::invalid_argument("Cannot move onto own server");
    }
    // download + delete
    // assuming player does not deal with deleting when downloading
    owner->download(link);
}

void Firewall::onEnter(LinkManager::LinkKey link) {
    if (link.player != owner) {
        // TODO: implement firewall + add decorator using method later
        // void Firewall::onEnter(Link & link) {
        //     if (link.player != owner) {
        //         link = std::make_shared<Link>(new RevealDecorator(link));
        //     }
        //     base->onEnter(link);
        // }
    }
}

void Goal::onEnter(LinkManager::LinkKey link) {
    if (link.player == owner) {
        throw std::invalid_argument("Cannot move onto own server");
    }
    link.player->download(link);
}
