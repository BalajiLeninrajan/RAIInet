#include "link.h"

#include <memory>

#include "board.h"

// Base Link
Link::Link(std::pair<int, int> startCoords, int strength, Player* owner,
           Board* board)
    : coords(startCoords), owner(owner), board(board), strength(strength) {}

Link::~Link() {}

int Link::getStrength() const { return strength; }

bool Link::getRevealState() const { return false; }

std::pair<int, int> Link::getCoords() const { return coords; }

void Link::setCoords(std::pair<int, int> newCoords) { coords = newCoords; }

void Link::requestMove(Link::Direction dir) {
    switch (dir) {
        case Direction::NORTH:
            board->moveLink(coords, {coords.first + 1, coords.second});
            break;
        case Direction::SOUTH:
            board->moveLink(coords, {coords.first - 1, coords.second});
            break;
        case Direction::EAST:
            board->moveLink(coords, {coords.first, coords.second + 1});
            break;
        case Direction::WEST:
            board->moveLink(coords, {coords.first, coords.second - 1});
            break;
    }
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

Link::LinkType DataLink::getType() const { return Link::LinkType::VIRUS; }

DataLink::~DataLink() {}
