#include "link.h"

#include <format>
#include <memory>
#include <stdexcept>

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
