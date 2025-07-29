#include "cell.h"

#include <memory>
#include <stdexcept>

#include "link.h"
#include "player.h"

void BaseCell::onEnter(Link& link) {}

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

void BaseCell::emptyCell() { linkKey.reset(); }

BoardCell::BoardCell(std::shared_ptr<LinkManager> lm) : BaseCell(lm) {}
BoardCell::~BoardCell() {}

// onEnter should check for collision and handle it
void BoardCell::onEnter(Link& link) {
    if (link.getOwner() == getOccupantLink()->getOwner()) {
        throw std::invalid_argument("Cannot move onto own link");
    }
    if (getOccupantLink() == nullptr) {
        setOccupantLink(std::make_shared<Link>(link));
        return;
    }
    // handles battle, winner downloads loser and loser gets deleted
    if (link->getStrength() >= getOccupantLink()->getStrength()) {
        link->getOwner()->download(*getOccupantLink());
        getOccupantLink()->getOwner()->deleteLink(*getOccupantLink());
        getOccupantLink() = link;
        return;
    }
    getOccupantLink()->getOwner()->download(*link);
    link->getOwner()->deleteLink(*link);
}

PlayerCell::PlayerCell(std::unique_ptr<BaseCell> base, Player& owner)
    : base{std::move(base)}, owner{&owner} {}

PlayerCell::~PlayerCell() {}

void Server::onEnter(Link& link) {
    if (link.getOwner() == owner) {
        throw std::invalid_argument("Cannot move onto own server");
    }
    // download + delete
    // assuming player does not deal with deleting when downloading
    owner->download(*link);
}

void Firewall::onEnter(std::shared_ptr<Link>& link) {
    if (link->getOwner() != owner) {
        void Firewall::onEnter(Link & link) {
            if (link.getOwner() != owner) {
                link = std::make_shared<Link>(new RevealDecorator(link));
            }
            base->onEnter(link);
        }

        void Goal::onEnter(std::shared_ptr<Link> & link) {
            if (link->getOwner() == owner) {
                throw std::invalid_argument("Cannot move onto own server");
            }
            link->getOwner()->download(*link);
        }
