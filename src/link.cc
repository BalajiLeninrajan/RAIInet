#include "link.h"

#include <memory>

#include "board.h"

// Base Link
Link::Link(std::pair<int, int> startCoords, int strength,
           std::unique_ptr<Player>& owner, std::unique_ptr<Board>& board)
    : coords(startCoords),
      owner(owner.get()),
      board(board.get()),
      strength(strength) {}

int Link::getStrength() const { return strength; }

bool Link::getRevealState() const { return false; }

std::pair<int, int> Link::getCoords() const { return coords; }

void Link::setCoords(std::pair<int, int> newCoords) { coords = newCoords; }

void Link::requestMove(Link::Direction dir) {}

Player* Link::getOwner() const { return owner; }

// VirusLink

VirusLink::VirusLink(std::pair<int, int> startCoords, int strength,
                     std::unique_ptr<Player>& owner,
                     std::unique_ptr<Board>& board)
    : Link(startCoords, strength, owner, board) {}

Link::LinkType VirusLink::getType() const { return Link::LinkType::VIRUS; }

// DataLink

DataLink::DataLink(std::pair<int, int> startCoords, int strength,
                   std::unique_ptr<Player>& owner,
                   std::unique_ptr<Board>& board)
    : Link(startCoords, strength, owner, board) {}

Link::LinkType DataLink::getType() const { return Link::LinkType::VIRUS; }
