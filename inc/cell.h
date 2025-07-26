#pragma once

#include <memory>

class Link;
class Player;

// Abstract base class for all cells on the board.
class BaseCell {
    Link *occupantLink =
        nullptr;  // Non-owning pointer to the link on this cell.

   public:
    virtual ~BaseCell() = default;
    virtual void onEnter(
        Link &link) = 0;  // Action to perform when a link enters.
};

// A standard, unoccupied cell on the board.
class BoardCell : public BaseCell {
   public:
    void onEnter(Link &link) override;
};

// Abstract decorator for cells owned by a player.
class PlayerCell : public BaseCell {
   protected:
    std::unique_ptr<BaseCell> base;  // The cell being decorated.
    Player *owner;  // Non-owning pointer to the player who owns this cell.

   public:
    PlayerCell(std::unique_ptr<BaseCell> base, Player *owner)
        : base{std::move(base)}, owner{owner} {}
};

// Represents a player's Server Port.
class Server : public PlayerCell {
   public:
    using PlayerCell::PlayerCell;
    void onEnter(Link &link) override;
};

// Represents a player's Firewall.
class Firewall : public PlayerCell {
   public:
    using PlayerCell::PlayerCell;
    void onEnter(Link &link) override;
};

// Represents the opponent's goal area (Server Ports).
class Goal : public PlayerCell {
   public:
    using PlayerCell::PlayerCell;
    void onEnter(Link &link) override;
};
