#include "cell.h"

#include <memory>
#include <stdexcept>

#include "link.h"
#include "player.h"

void BaseCell::onEnter(std::shared_ptr<Link>& link) {}
std::shared_ptr<Link>& BaseCell::getOccupantLink() { return occupantLink; }

BoardCell::BoardCell() {}
BoardCell::~BoardCell() {}

// onEnter should check for collision and handle it
void BoardCell::onEnter(std::shared_ptr<Link>& link) {
    if (link->getOwner() == occupantLink->getOwner()) {
        throw std::invalid_argument("Cannot move onto own link");
    }
    if (occupantLink == nullptr) {
        occupantLink = std::make_shared<Link>(link);
        return;
    }
    // handles battle, winner downloads loser and loser gets deleted
    if (link->getStrength() >= occupantLink->getStrength()) {
        link->getOwner()->download(*occupantLink);
        occupantLink->getOwner()->deleteLink(*occupantLink);
        occupantLink = link;
        return;
    }
    occupantLink->getOwner()->download(*link);
    link->getOwner()->deleteLink(*link);
}

PlayerCell::PlayerCell(std::unique_ptr<BaseCell> base,
                       std::weak_ptr<Player> owner)
    : base{std::move(base)}, owner{owner} {}

PlayerCell::~PlayerCell() {}

void Server::onEnter(std::shared_ptr<Link>& link) {
    if (link->getOwner() == owner.lock()) {
        throw std::invalid_argument("Cannot move onto own server");
    }
    // download + delete
    // assuming player does not deal with deleting when downloading
    owner.lock()->download(*link);
    link->getOwner()->download(*link);
}

void Firewall::onEnter(std::shared_ptr<Link>& link) {
    if (link->getOwner() != owner.lock()) {
        link = std::make_shared<Link>(new RevealDecorator(link));
    }
    base->onEnter(link);
}

void Goal::onEnter(std::shared_ptr<Link>& link) {
    if (link->getOwner() == owner.lock()) {
        throw std::invalid_argument("Cannot move onto own server");
    }
    link->getOwner()->download(*link);
}
