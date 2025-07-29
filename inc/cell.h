#pragma once

#include <memory>
#include <optional>

#include "linkmanager.h"

class Link;
class Player;

// Abstract base class for all cells on the board.
class BaseCell {
    std::optional<LinkManager::LinkKey> linkKey;
    std::shared_ptr<LinkManager> linkManager;

   protected:
    virtual Link &getLink(LinkManager::LinkKey link);

   public:
    BaseCell(std::shared_ptr<LinkManager> lm) : linkManager{lm} {}
    virtual ~BaseCell() = default;
    virtual void onEnter(LinkManager::LinkKey link) = 0;
    virtual LinkManager::LinkKey getOccupantLink();
    virtual void setOccupantLink(LinkManager::LinkKey new_link);
    virtual bool isOccupied();
    virtual void emptyCell();
};

// A standard, unoccupied cell on the board.
class BoardCell : public BaseCell {
   public:
    BoardCell(std::shared_ptr<LinkManager> lm);
    ~BoardCell();
    void onEnter(LinkManager::LinkKey link) override;
};

// Abstract decorator for cells owned by a player.
class PlayerCell : public BaseCell {
   protected:
    std::unique_ptr<BaseCell> base;
    Player *owner;

   public:
    PlayerCell(std::unique_ptr<BaseCell> base, Player &owner);
    virtual ~PlayerCell();
};

// Represents a player's Server Port.
class Server : public PlayerCell {
   public:
    using PlayerCell::PlayerCell;
    void onEnter(LinkManager::LinkKey link) override;
};

// Represents a player's Firewall.
class Firewall : public PlayerCell {
   public:
    using PlayerCell::PlayerCell;
    void onEnter(LinkManager::LinkKey link) override;
};

// Represents the opponent's goal area (Server Ports).
class Goal : public PlayerCell {
   public:
    using PlayerCell::PlayerCell;
    void onEnter(LinkManager::LinkKey link) override;
};
