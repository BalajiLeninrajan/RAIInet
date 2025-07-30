#include "link.h"

#include <memory>
#include <stdexcept>

#include "board.h"
#include "linkmanager.h"

// Base Link
Link::Link(std::pair<int, int> startCoords, int strength, Player* owner,
           Board* board)
    : coords(startCoords), owner(owner), board(board), strength(strength) {}

Link::~Link() {}

int Link::getStrength() const { return strength; }

bool Link::getRevealState() const { return false; }

std::pair<int, int> Link::getCoords() const { return coords; }

void Link::setCoords(std::pair<int, int> newCoords) { coords = newCoords; }

std::pair<int, int> Link::getNewCoords(std::pair<int, int> coords,
                                       Link::Direction dir) {
    switch (dir) {
        case Link::Direction::NORTH:
            coords.first--;
            break;
        case Link::Direction::SOUTH:
            coords.first++;
            break;
        case Link::Direction::EAST:
            coords.second++;
            break;
        case Link::Direction::WEST:
            coords.second--;
            break;
    }
    return coords;
}

void Link::requestMove(Link::Direction dir) {
    board->moveLink(coords, getNewCoords(coords, dir));
}

Link::Direction Link::charToDirection(char c) {
    switch (c) {
        case 'N':
            return Direction::NORTH;
        case 'S':
            return Direction::SOUTH;
        case 'E':
            return Direction::EAST;
        case 'W':
            return Direction::WEST;
    }

    std::string debugmsg = "No direction associated with char ";
    debugmsg += c;
    throw std::invalid_argument(debugmsg);
}
Player* Link::getOwner() const { return owner; }

// VirusLink

VirusLink::VirusLink(std::pair<int, int> startCoords, int strength,
                     Player* owner, Board* board)
    : Link(startCoords, strength, owner, board) {}

Link::LinkType VirusLink::getType() const { return Link::LinkType::VIRUS; }

// DataLink

DataLink::DataLink(std::pair<int, int> startCoords, int strength, Player* owner,
                   Board* board)
    : Link(startCoords, strength, owner, board) {}

Link::LinkType DataLink::getType() const { return Link::LinkType::DATA; }

DataLink::~DataLink() {}

// Link decorator

LinkDecorator::LinkDecorator(std::unique_ptr<Link> base)
    : Link(base->getCoords(), base->getStrength(), nullptr, nullptr),
      base(std::move(base)) {}

Link::LinkType LinkDecorator::getType() const { return base->getType(); }

bool LinkDecorator::getRevealState() const { return base->getRevealState(); }

std::pair<int, int> LinkDecorator::getCoords() const {
    return base->getCoords();
}

void LinkDecorator::setCoords(std::pair<int, int> newCoords) {
    base->setCoords(newCoords);
}

std::pair<int, int> LinkDecorator::getNewCoords(std::pair<int, int> coords,
                                                Link::Direction dir) {
    return base->getNewCoords(coords, dir);
}

Player* LinkDecorator::getOwner() const { return base->getOwner(); }

// Link boosts
std::pair<int, int> LinkBoostDecorator::getNewCoords(std::pair<int, int> coords,
                                                     Link::Direction dir) {
    return base->getNewCoords(coords, dir);
}

// Polarize
Link::LinkType PolarizeDecorator::getType() const {
    switch (base->getType()) {
        case Link::LinkType::VIRUS:
            return Link::LinkType::DATA;
        default:
            return Link::LinkType::VIRUS;
    }
}

// Reveal
bool RevealDecorator::getRevealState() const { return true; }

// Lag
// kys

// QuantumEntanglement
QuantumEntanglementDecorator::QuantumEntanglementDecorator(
    std::unique_ptr<Link> base, Link* partner)
    : LinkDecorator(std::move(base)), partner(partner) {}

void QuantumEntanglementDecorator::requestMove(Direction dir) {
    base->requestMove(dir);
    try {
        partner->requestMove(dir);
    } catch (...) {
    }
}
