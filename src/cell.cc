#include "cell.h"

#include <memory>
#include <stdexcept>

#include "link.h"
#include "player.h"

void BaseCell::onEnter(std::shared_ptr<Link>& link) {}
std::shared_ptr<Link> BaseCell::getOccupantLink() { return occupantLink; }
void BaseCell::setOccupantLink(std::shared_ptr<Link> new_link) {
    occupantLink = new_link;
}

BoardCell::BoardCell() {}
BoardCell::~BoardCell() {}

// onEnter should check for collision and handle it
void BoardCell::onEnter(std::shared_ptr<Link>& link) {
    if (link->getOwner() == getOccupantLink()->getOwner()) {
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

void Server::onEnter(std::shared_ptr<Link>& link) {
    if (link->getOwner() == owner) {
        throw std::invalid_argument("Cannot move onto own server");
    }
    // download + delete
    // assuming player does not deal with deleting when downloading
    owner->download(*link);
}

void Firewall::onEnter(std::shared_ptr<Link>& link) {
    if (link->getOwner() != owner) {
        link = std::make_shared<Link>(new RevealDecorator(link));
    }
    base->onEnter(link);
}

void Goal::onEnter(std::shared_ptr<Link>& link) {
    if (link->getOwner() == owner) {
        throw std::invalid_argument("Cannot move onto own server");
    }
    link->getOwner()->download(*link);
}
