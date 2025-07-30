#pragma once

#include <memory>
#include <optional>

#include "game.h"
#include "linkmanager.h"

class Link;
class Player;

// Abstract base class for all cells on the board.
class BaseCell {
    std::optional<LinkManager::LinkKey> linkKey;

   public:
    virtual ~BaseCell() = default;
    virtual void onEnter(LinkManager::LinkKey link, Game *game) = 0;
    virtual LinkManager::LinkKey getOccupantLink() const;
    virtual void setOccupantLink(LinkManager::LinkKey new_link);
    virtual bool isOccupied() const;
    virtual void emptyCell();
    virtual std::string cellRepresentation(const Game *game) const;
    friend class PlayerCell;
    friend class Board;
    virtual bool canDecorate() const;
};

// A standard, unoccupied cell on the board.
class BoardCell : public BaseCell {
   public:
    ~BoardCell();
    void onEnter(LinkManager::LinkKey link, Game *game) override;
};

// Abstract decorator for cells owned by a player.
class PlayerCell : public BaseCell {
   protected:
    std::unique_ptr<BaseCell> base;
    Player *owner;

   public:
    PlayerCell(std::unique_ptr<BaseCell> base, Player *owner);
    virtual ~PlayerCell() = 0;
    LinkManager::LinkKey getOccupantLink() const override;
    void setOccupantLink(LinkManager::LinkKey new_link) override;
    bool isOccupied() const override;
    void emptyCell() override;

    friend class Board;
};

// Represents a player's Server Port.
class Server : public PlayerCell {
   public:
    using PlayerCell::PlayerCell;
    void onEnter(LinkManager::LinkKey link, Game *game) override;
    std::string cellRepresentation(const Game *game) const override;
    bool canDecorate() const override;
};

// Represents a player's Firewall.
class Firewall : public PlayerCell {
   public:
    using PlayerCell::PlayerCell;
    ~Firewall();
    void onEnter(LinkManager::LinkKey link, Game *game) override;
    std::string cellRepresentation(const Game *game) const override;
};

// Represents the opponent's goal area (Server Ports).
class Goal : public PlayerCell {
   public:
    using PlayerCell::PlayerCell;
    void onEnter(LinkManager::LinkKey link, Game *game) override;
    std::string cellRepresentation(const Game *game) const override;
    bool canDecorate() const override;
};
