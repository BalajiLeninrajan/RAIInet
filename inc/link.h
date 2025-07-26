#pragma once

#include "enums.h"
#include <memory>
#include <utility>

class Board;

// Abstract base class for player-controlled links (pieces).
class Link {
protected:
  std::pair<int, int> coords;
  int strength;
  LinkType type;
  bool isRevealed = false;

public:
  virtual ~Link() = default;
  virtual int getStrength() const { return strength; }
  virtual LinkType getType() const { return type; }

  std::pair<int, int> getCoords() const { return coords; }
  void setCoords(std::pair<int, int> newCoords) { coords = newCoords; }
  bool getRevealState() const { return isRevealed; }

  // The Game class will handle the logic for this.
  void requestMove(Direction dir);
};

// A concrete implementation for a Virus link.
class VirusLink : public Link {
public:
  VirusLink(std::pair<int, int> startCoords);
};

// A concrete implementation for a Data link.
class DataLink : public Link {
public:
  DataLink(std::pair<int, int> startCoords);
};

// Abstract decorator for Links to add abilities.
class LinkDecorator : public Link {
protected:
  std::shared_ptr<Link> base; // Shared ownership of the link being decorated.

public:
  LinkDecorator(std::shared_ptr<Link> base) : base{std::move(base)} {}
  int getStrength() const override { return base->getStrength(); }
};

class LinkBoostDecorator : public LinkDecorator {
public:
  using LinkDecorator::LinkDecorator;
  int getStrength() const override;
};

class PolarizeDecorator : public LinkDecorator {
public:
  using LinkDecorator::LinkDecorator;
};

class RevealDecorator : public LinkDecorator {
public:
  using LinkDecorator::LinkDecorator;
};

class LagDecorator : public LinkDecorator {
  int turnsLeft;

public:
  using LinkDecorator::LinkDecorator;
};

class QuantumEntanglementDecorator : public LinkDecorator {
  std::shared_ptr<Link> partner; // Shares ownership with the entangled partner.
public:
  QuantumEntanglementDecorator(std::shared_ptr<Link> base,
                               std::shared_ptr<Link> partner)
      : LinkDecorator{std::move(base)}, partner{std::move(partner)} {}
};
