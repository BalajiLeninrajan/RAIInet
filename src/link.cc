#include "link.h"

#include <memory>

// Base Link

Link::Link(std::pair<int, int> startCoords, int strength,
           std::weak_ptr<Player> owner)
    : coords(startCoords), owner(owner), strength(strength) {}

int Link::getStrength() const { return strength; }

bool Link::getRevealState() const { return false; }

std::pair<int, int> Link::getCoords() const { return coords; }

void Link::setCoords(std::pair<int, int> newCoords) { coords = newCoords; }

std::weak_ptr<Player> Link::getOwner() const { return owner; }

// VirusLink

VirusLink::VirusLink(std::pair<int, int> startCoords, int strength,
                     std::weak_ptr<Player> owner)
    : Link(startCoords, strength, owner) {}

Link::LinkType VirusLink::getType() const { return Link::LinkType::VIRUS; }

// DataLink

DataLink::DataLink(std::pair<int, int> startCoords, int strength,
                   std::weak_ptr<Player> owner)
    : Link(startCoords, strength, owner) {}

Link::LinkType DataLink::getType() const { return Link::LinkType::VIRUS; }
