#pragma once

#include <memory>
#include <utility>

// Abstract base class for player-controlled links (pieces).
class Link {
   protected:
    std::pair<int, int> coords;
    int strength;

   public:
    enum class Direction { NORTH, SOUTH, EAST, WEST };
    enum class LinkType { VIRUS, DATA };

    virtual ~Link() = default;
    int getStrength() const;

    virtual LinkType getType() const;
    virtual bool getRevealState() const;

    std::pair<int, int> getCoords() const;
    void setCoords(std::pair<int, int> newCoords);

    // The Game class will handle the logic for this.
    virtual void requestMove(Direction dir);
};

// A concrete implementation for a Virus link.
class VirusLink : public Link {
   public:
    VirusLink(std::pair<int, int> startCoords);
    virtual LinkType getType() const override;
};

// A concrete implementation for a Data link.
class DataLink : public Link {
   public:
    DataLink(std::pair<int, int> startCoords);
    virtual LinkType getType() const override;
};

// Abstract decorator for Links to add abilities.
class LinkDecorator : public Link {
   protected:
    std::shared_ptr<Link> base;

   public:
    LinkDecorator(std::shared_ptr<Link> base);
};

class LinkBoostDecorator : public LinkDecorator {
   public:
    using LinkDecorator::LinkDecorator;
    void requestMove(Direction dir) override;
};

class PolarizeDecorator : public LinkDecorator {
   public:
    using LinkDecorator::LinkDecorator;
    LinkType getType() const override;
};

class RevealDecorator : public LinkDecorator {
   public:
    using LinkDecorator::LinkDecorator;
    bool getRevealState() const override;
};

class LagDecorator : public LinkDecorator {
    int turnsLeft;

   public:
    using LinkDecorator::LinkDecorator;
    void requestMove(Direction dir) override;
};

class QuantumEntanglementDecorator : public LinkDecorator {
    std::shared_ptr<Link> partner;

   public:
    QuantumEntanglementDecorator(std::shared_ptr<Link> base,
                                 std::shared_ptr<Link> partner);
};
