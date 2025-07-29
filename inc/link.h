#pragma once

#include <memory>
#include <utility>

class Player;
class Board;

// Abstract base class for player-controlled links (pieces).
class Link {
   protected:
    std::pair<int, int> coords;
    Player* owner;
    Board* board;
    int strength;

   public:
    enum class Direction { NORTH, SOUTH, EAST, WEST };
    enum class LinkType { VIRUS, DATA };

    static Direction charToDirection(char c);

    Link(std::pair<int, int> startCoords, int strength, Player* player,
         Board* board);
    virtual ~Link() = 0;
    int getStrength() const;

    virtual LinkType getType() const = 0;
    virtual bool getRevealState() const;

    virtual std::pair<int, int> getCoords() const;
    virtual void setCoords(std::pair<int, int> newCoords);

    // The Game class will handle the logic for this.
    virtual void requestMove(Direction dir);

    virtual Player* getOwner() const;

    virtual std::pair<int, int> getNewCoords(std::pair<int, int> coords,
                                             Link::Direction dir);
};

// A concrete implementation for a Virus link.
class VirusLink : public Link {
   public:
    VirusLink(std::pair<int, int> startCoords, int strength, Player* owner,
              Board* board);
    virtual LinkType getType() const override;
};

// A concrete implementation for a Data link.
class DataLink : public Link {
   public:
    DataLink(std::pair<int, int> startCoords, int strength, Player* owner,
             Board* board);
    virtual LinkType getType() const override;
    virtual ~DataLink() override;
};

// Abstract decorator for Links to add abilities.
class LinkDecorator : public Link {
   protected:
    std::unique_ptr<Link> base;

   public:
    LinkDecorator(std::unique_ptr<Link> base);

    LinkType getType() const override;
    bool getRevealState() const override;

    std::pair<int, int> getCoords() const override;
    void setCoords(std::pair<int, int> newCoords) override;

    Player* getOwner() const override;

    virtual std::pair<int, int> getNewCoords(std::pair<int, int> coords,
                                             Link::Direction dir) override;
};

class LinkBoostDecorator : public LinkDecorator {
   public:
    using LinkDecorator::LinkDecorator;

    std::pair<int, int> getNewCoords(std::pair<int, int> coords,
                                     Link::Direction dir) override;
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
    Link* partner;

   public:
    QuantumEntanglementDecorator(std::unique_ptr<Link> base, Link* partner);
    void requestMove(Direction dir) override;
};
