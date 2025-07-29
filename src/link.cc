#include "link.h"

#include <memory>

#include "board.h"

// Base Link
<<<<<<< HEAD
Link::Link(std::pair<int, int> startCoords, int strength,
<<<<<<< HEAD
           std::unique_ptr<Player>& owner, std::unique_ptr<Board>& board)
    : coords(startCoords),
      owner(owner.get()),
      board(board.get()),
      strength(strength) {}

=======
           Player* owner, Board* board)
    : coords(startCoords),
      owner(owner),
      board(board),
      strength(strength) {}
=======
Link::Link(std::pair<int, int> startCoords, int strength, Player* owner,
           Board* board)
    : coords(startCoords), owner(owner), board(board), strength(strength) {}
>>>>>>> origin/main

Link::~Link() {}

>>>>>>> origin/main
int Link::getStrength() const { return strength; }

bool Link::getRevealState() const { return false; }

std::pair<int, int> Link::getCoords() const { return coords; }

void Link::setCoords(std::pair<int, int> newCoords) { coords = newCoords; }

void Link::requestMove(Link::Direction dir) {
    // TODO: call move link on board
}

Player* Link::getOwner() const { return owner; }

// VirusLink

VirusLink::VirusLink(std::pair<int, int> startCoords, int strength,
<<<<<<< HEAD
<<<<<<< HEAD
                     std::unique_ptr<Player>& owner,
                     std::unique_ptr<Board>& board)
=======
                     Player* owner,
                     Board* board)
>>>>>>> origin/main
=======
                     Player* owner, Board* board)
>>>>>>> origin/main
    : Link(startCoords, strength, owner, board) {}

Link::LinkType VirusLink::getType() const { return Link::LinkType::VIRUS; }

// DataLink

<<<<<<< HEAD
DataLink::DataLink(std::pair<int, int> startCoords, int strength,
<<<<<<< HEAD
                   std::unique_ptr<Player>& owner,
                   std::unique_ptr<Board>& board)
    : Link(startCoords, strength, owner, board) {}

Link::LinkType DataLink::getType() const { return Link::LinkType::VIRUS; }
=======
                   Player* owner,
=======
DataLink::DataLink(std::pair<int, int> startCoords, int strength, Player* owner,
>>>>>>> origin/main
                   Board* board)
    : Link(startCoords, strength, owner, board) {}

Link::LinkType DataLink::getType() const { return Link::LinkType::VIRUS; }

DataLink::~DataLink() {}
>>>>>>> origin/main
