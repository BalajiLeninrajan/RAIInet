// cell.h
#pragma once

#include <memory>
#include <optional>
#include <string>

#include "linkmanager.h"

// Forward declarations to avoid circular includes
class Link;
class Player;
class Game;

/**
 * @brief Abstract base class for all cells on the game board.
 *
 * Defines the common interface and properties for any cell on the board,
 * including occupancy by a link and behavior when a link enters it.
 */
class BaseCell {
    std::optional<LinkManager::LinkKey>
        linkKey; /**< Stores the LinkKey of the link occupying the cell, if any.
                  */

   public:
    /**
     * @brief Virtual destructor for BaseCell.
     */
    virtual ~BaseCell() = default;

    /**
     * @brief Pure virtual function called when a link enters this cell.
     *
     * Concrete cell types must implement this to define their specific
     * interaction logic (e.g., scoring, battle, blocking).
     *
     * @param link The LinkManager::LinkKey of the link that entered the cell.
     * @param game A pointer to the Game instance, allowing cell interactions to
     * affect game state.
     */
    virtual void onEnter(LinkManager::LinkKey link, Game *game) = 0;

    /**
     * @brief Gets the LinkKey of the link currently occupying this cell.
     * @return The LinkManager::LinkKey of the occupant link, throws and error
     * if link not found
     */
    virtual LinkManager::LinkKey getOccupantLink() const;

    /**
     * @brief Sets the link that occupies this cell.
     * @param new_link The LinkManager::LinkKey of the new link occupying the
     * cell.
     */
    virtual void setOccupantLink(LinkManager::LinkKey new_link);

    /**
     * @brief Checks if the cell is currently occupied by a link.
     * @return True if the cell is occupied, false otherwise.
     */
    virtual bool isOccupied() const;

    /**
     * @brief Empties the cell of any occupying link.
     */
    virtual void emptyCell();

    /**
     * @brief Returns a string representation of the cell for display.
     * @param game A const pointer to the Game instance, used to retrieve link
     * details for representation.
     * @return A string representing the cell's current state.
     */
    virtual std::string cellRepresentation(const Game *game) const;

    friend class PlayerCell; /**< PlayerCell needs access to protected members
                                for decoration. */
    friend class Board; /**< Board needs access for managing cell occupancy. */

    /**
     * @brief Checks if this cell can be decorated (e.g., by a Firewall).
     * @return True if the cell can be decorated, false otherwise.
     */
    virtual bool canDecorate() const;
};

/**
 * @brief A standard, unoccupied cell on the board.
 *
 * Links can pass through BoardCells without special effects.
 */
class BoardCell : public BaseCell {
   public:
    /**
     * @brief Destructor for BoardCell.
     */
    ~BoardCell();

    /**
     * @brief Handles a link entering a BoardCell.
     *
     * Typically, a link just occupies the cell without any special interaction.
     * Will battle if there is an opponent link on the cell and will reject if
     * own link is on the cell
     *
     * @param link The LinkManager::LinkKey of the link that entered.
     * @param game A pointer to the Game instance.
     */
    void onEnter(LinkManager::LinkKey link, Game *game) override;
};

/**
 * @brief Abstract decorator for cells owned by a player.
 *
 * This class serves as the base for various player-specific cell types (Server,
 * Firewall, Goal) that modify the behavior or representation of an underlying
 * BaseCell.
 */
class PlayerCell : public BaseCell {
   protected:
    std::unique_ptr<BaseCell>
        base;      /**< A unique_ptr to the BaseCell being decorated. */
    Player *owner; /**< A pointer to the Player who owns this cell. */

   public:
    /**
     * @brief Constructor for PlayerCell.
     * @param base A unique_ptr to the BaseCell that this PlayerCell will
     * decorate.
     * @param owner A pointer to the Player who owns this cell.
     */
    PlayerCell(std::unique_ptr<BaseCell> base, Player *owner);

    /**
     * @brief Pure virtual destructor for PlayerCell.
     */
    virtual ~PlayerCell() = 0;

    /**
     * @brief Delegates to the base cell to get the occupant link.
     * @return The LinkManager::LinkKey of the occupant link.
     */
    LinkManager::LinkKey getOccupantLink() const override;

    /**
     * @brief Delegates to the base cell to set the occupant link.
     * @param new_link The LinkManager::LinkKey of the new link.
     */
    void setOccupantLink(LinkManager::LinkKey new_link) override;

    /**
     * @brief Delegates to the base cell to check if it's occupied.
     * @return True if occupied, false otherwise.
     */
    bool isOccupied() const override;

    /**
     * @brief Delegates to the base cell to empty it.
     */
    void emptyCell() override;

    friend class Board; /**< Board needs access to modify decorated cells. */
};

/**
 * @brief Represents a player's Server Port.
 *
 * This is a critical cell type. When an opponent's link enters a Server,
 * it typically results in scoring or capture.
 */
class Server : public PlayerCell {
   public:
    using PlayerCell::PlayerCell;  // Inherit constructors from PlayerCell

    /**
     * @brief Handles a link entering a Server cell.
     *
     * When an opponent's link enters a Server cell, it is usually "downloaded"
     * and contributes to the owning player's score.
     *
     * @param link The LinkManager::LinkKey of the link that entered.
     * @param game A pointer to the Game instance.
     */
    void onEnter(LinkManager::LinkKey link, Game *game) override;

    /**
     * @brief Returns the string representation of a Server cell.
     * @param game A const pointer to the Game instance.
     * @return A string representing the Server cell, possibly showing its
     * owner.
     */
    std::string cellRepresentation(const Game *game) const override;

    /**
     * @brief Indicates if a Server cell can't be decorated.
     * @return False, indicating a Server can't be decorated (e.g., by a
     * Firewall).
     */
    bool canDecorate() const override;
};

/**
 * @brief Represents a player's Firewall.
 *
 * Opponent links are revealed and, if they are viruses, immediately downloaded
 * by their owner
 *
 */
class Firewall : public PlayerCell {
   public:
    using PlayerCell::PlayerCell;  // Inherit constructors from PlayerCell

    /**
     * @brief Destructor for Firewall.
     */
    ~Firewall();

    /**
     * @brief Handles a link entering a Firewall cell.
     *
     * Opponent links are revealed and, if they are viruses, immediately
     * downloaded by their owner
     *
     * @param link The LinkManager::LinkKey of the link that entered.
     * @param game A pointer to the Game instance.
     */
    void onEnter(LinkManager::LinkKey link, Game *game) override;

    /**
     * @brief Returns the string representation of a Firewall cell.
     * @param game A const pointer to the Game instance.
     * @return A string representing the Firewall cell.
     */
    std::string cellRepresentation(const Game *game) const override;
};

/**
 * @brief Represents the opponent's goal area (their Server Ports).
 *
 * This cell type is essentially a decorated Server belonging to an opponent,
 * signifying an area a player wants to reach with their links to score.
 */
class Goal : public PlayerCell {
   public:
    using PlayerCell::PlayerCell;  // Inherit constructors from PlayerCell

    /**
     * @brief Handles a link entering a Goal cell.
     *
     * When a player's link enters an opponent's Goal cell, it results in
     * scoring for the player who owns the entering link.
     *
     * @param link The LinkManager::LinkKey of the link that entered.
     * @param game A pointer to the Game instance.
     */
    void onEnter(LinkManager::LinkKey link, Game *game) override;

    /**
     * @brief Returns the string representation of a Goal cell.
     * @param game A const pointer to the Game instance.
     * @return A string representing the Goal cell.
     */
    std::string cellRepresentation(const Game *game) const override;

    /**
     * @brief Indicates if a Goal can't be decorated.
     * @return False, indicating a Goal can't be decorated.
     */
    bool canDecorate() const override;
};
