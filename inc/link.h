// link.h
#pragma once

#include <memory>
#include <utility>  // For std::pair

class Player;
class Board;
class Game;

/**
 * @brief Abstract base class for player-controlled links (game pieces).
 *
 * Defines the common properties and behaviors for all links, such as position,
 * owner, strength, and movement capabilities. This is the Component in the
 * Decorator pattern.
 */
class Link {
   protected:
    std::pair<int, int> coords; /**< Current coordinates (row, column) of the
                                   link on the board. */
    Player* owner; /**< Pointer to the Player who owns this link. */
    Board* board;  /**< Pointer to the game Board instance. */
    int strength;  /**< Strength value of the link, used in battles. */

   public:
    /**
     * @brief Enum for defining movement directions.
     */
    enum class Direction { NORTH, SOUTH, EAST, WEST };

    /**
     * @brief Enum for defining link types.
     */
    enum class LinkType { VIRUS, DATA };

    /**
     * @brief Static helper to convert a character (e.g., 'N', 'S') to a
     * Direction enum.
     * @param c The character representing the direction.
     * @return The corresponding Direction enum value.
     */
    static Direction charToDirection(char c);

    /**
     * @brief Constructor for the Link class.
     * @param startCoords The initial coordinates of the link.
     * @param strength The strength of the link.
     * @param player A pointer to the Player who owns this link.
     * @param board A pointer to the game Board instance.
     */
    Link(std::pair<int, int> startCoords, int strength, Player* player,
         Board* board);

    /**
     * @brief Pure virtual destructor for the Link class.
     */
    virtual ~Link() = 0;

    /**
     * @brief Gets the strength of the link.
     * @return The integer strength value.
     */
    int getStrength() const;

    /**
     * @brief Pure virtual function to get the link's type (Virus or Data).
     * @return The LinkType of the link.
     */
    virtual LinkType getType() const = 0;

    /**
     * @brief Checks if the link is revealed to opponents.
     * @return True if the link is revealed, false otherwise.
     */
    virtual bool getRevealState() const;

    /**
     * @brief Gets the current coordinates of the link.
     * @return A pair of integers representing the (row, column) coordinates.
     */
    virtual std::pair<int, int> getCoords() const;

    /**
     * @brief Sets the current coordinates of the link.
     * @param newCoords A pair of integers representing the new (row, column)
     * coordinates.
     */
    virtual void setCoords(std::pair<int, int> newCoords);

    /**
     * @brief Requests a move for the link in a given direction.
     *
     * The Game class will handle the specific logic for this move,
     * including boundary checks and interactions with other cells/links.
     *
     * @param dir The Direction enum indicating the desired movement.
     * @param game A pointer to the Game instance to facilitate movement logic.
     */
    virtual void requestMove(Direction dir, Game* game);

    /**
     * @brief Gets the owner of the link.
     * @return A pointer to the Player who owns this link.
     */
    virtual Player* getOwner() const;

    /**
     * @brief Calculates the new coordinates if the link moves in a given
     * direction.
     * @param coords The starting coordinates for the calculation.
     * @param dir The Direction of movement.
     * @return A pair of integers representing the calculated new (row, column)
     * coordinates.
     */
    virtual std::pair<int, int> getNewCoords(std::pair<int, int> coords,
                                             Link::Direction dir);
    friend class LinkDecorator; /**< LinkDecorator needs access to protected
                                   members. */
};

/**
 * @brief A concrete implementation for a Virus link.
 *
 * Virus links have specific properties and behaviors, often related to
 * attacking opponent Data links or servers.
 */
class VirusLink : public Link {
   public:
    /**
     * @brief Constructor for VirusLink.
     * @param startCoords The initial coordinates.
     * @param strength The strength of the link.
     * @param owner A pointer to the owning Player.
     * @param board A pointer to the game Board.
     */
    VirusLink(std::pair<int, int> startCoords, int strength, Player* owner,
              Board* board);

    /**
     * @brief Returns the type of the link, which is LinkType::VIRUS.
     * @return LinkType::VIRUS.
     */
    virtual LinkType getType() const override;
};

/**
 * @brief A concrete implementation for a Data link.
 *
 * Data links have specific properties and behaviors, often related to defending
 * or reaching opponent servers for scoring.
 */
class DataLink : public Link {
   public:
    /**
     * @brief Constructor for DataLink.
     * @param startCoords The initial coordinates.
     * @param strength The strength of the link.
     * @param owner A pointer to the owning Player.
     * @param board A pointer to the game Board.
     */
    DataLink(std::pair<int, int> startCoords, int strength, Player* owner,
             Board* board);

    /**
     * @brief Returns the type of the link, which is LinkType::DATA.
     * @return LinkType::DATA.
     */
    virtual LinkType getType() const override;

    /**
     * @brief Destructor for DataLink.
     */
    ~DataLink();
};

/**
 * @brief Abstract base class for adding abilities/modifications to links
 * (Decorator pattern).
 *
 * LinkDecorator allows for dynamically adding new behaviors to Link objects
 * without altering their fundamental structure. It delegates most calls to the
 * wrapped base Link.
 */
class LinkDecorator : public Link {
   protected:
    std::unique_ptr<Link>
        base; /**< A unique_ptr to the Link object being decorated. */

   public:
    /**
     * @brief Constructor for LinkDecorator.
     * @param base A unique_ptr to the Link to be decorated.
     */
    LinkDecorator(std::unique_ptr<Link> base);

    /**
     * @brief Delegates to the base link to get its type.
     * @return The LinkType of the base link.
     */
    LinkType getType() const override;

    /**
     * @brief Delegates to the base link to get its reveal state.
     * @return True if the base link is revealed, false otherwise.
     */
    bool getRevealState() const override;

    /**
     * @brief Delegates to the base link to get its coordinates.
     * @return The coordinates of the base link.
     */
    std::pair<int, int> getCoords() const override;

    /**
     * @brief Delegates to the base link to set its coordinates.
     * @param newCoords The new coordinates.
     */
    void setCoords(std::pair<int, int> newCoords) override;

    /**
     * @brief Delegates to the base link to request a move.
     * @param dir The direction of movement.
     * @param game A pointer to the Game instance.
     */
    void requestMove(Link::Direction dir, Game* game) override;

    /**
     * @brief Delegates to the base link to get its owner.
     * @return A pointer to the owner Player.
     */
    Player* getOwner() const override;

    /**
     * @brief Delegates to the base link to calculate new coordinates.
     * @param coords The starting coordinates.
     * @param dir The direction.
     * @return The calculated new coordinates.
     */
    virtual std::pair<int, int> getNewCoords(std::pair<int, int> coords,
                                             Link::Direction dir) override;
};

/**
 * @brief Decorator that modifies link movement, allowing it to move further.
 *
 * This decorator changes the `getNewCoords` behavior to calculate a position
 * that is further than a single step in a given direction.
 */
class LinkBoostDecorator : public LinkDecorator {
   public:
    using LinkDecorator::LinkDecorator;  // Inherit constructors

    /**
     * @brief Modifies coordinate calculation for boosted movement.
     * @param coords The starting coordinates.
     * @param dir The direction of movement.
     * @return The new coordinates after a boosted move.
     */
    std::pair<int, int> getNewCoords(std::pair<int, int> coords,
                                     Link::Direction dir) override;
};

/**
 * @brief Decorator that changes the effective type of a link.
 *
 * This decorator allows a link to temporarily or permanently change its
 * LinkType (e.g., from VIRUS to DATA or vice versa) for gameplay purposes.
 */
class PolarizeDecorator : public LinkDecorator {
   public:
    using LinkDecorator::LinkDecorator;  // Inherit constructors

    /**
     * @brief Returns the polarized LinkType, which may be different from the
     * base link's original type.
     * @return The modified LinkType.
     */
    LinkType getType() const override;
};

/**
 * @brief Decorator that makes a link visible to opponents.
 *
 * This decorator changes the `getRevealState` behavior to always return true,
 * effectively making the decorated link visible on views that respect this
 * state.
 */
class RevealDecorator : public LinkDecorator {
   public:
    using LinkDecorator::LinkDecorator;  // Inherit constructors

    /**
     * @brief Returns true, indicating the decorated link is revealed.
     * @return True.
     */
    bool getRevealState() const override;
};

/**
 * @brief Decorator that delays or restricts link movement for a certain number
 * of turns.
 *
 * This decorator modifies the `requestMove` behavior, potentially preventing
 * movement or imposing a penalty for a specified duration.
 */
class LagDecorator : public LinkDecorator {
    int turnsLeft; /**< Number of turns the lag effect will last. */

   public:
    using LinkDecorator::LinkDecorator;  // Inherit constructors

    /**
     * @brief Modifies movement based on lag, potentially preventing or delaying
     * it.
     * @param dir The desired direction of movement.
     * @param game A pointer to the Game instance.
     */
    void requestMove(Direction dir, Game* game) override;
};

/**
 * @brief Decorator that links the movement of two distinct links.
 *
 * When one entangled link moves, its partner also attempts to move in the same
 * direction.
 */
class QuantumEntanglementDecorator : public LinkDecorator {
    Link* partner; /**< Pointer to the entangled partner link. */

   public:
    /**
     * @brief Constructor for QuantumEntanglementDecorator.
     * @param base A unique_ptr to the Link being decorated.
     * @param partner A pointer to the partner Link that will move together with
     * the base link.
     */
    QuantumEntanglementDecorator(std::unique_ptr<Link> base, Link* partner);

    /**
     * @brief Moves both the base link and its entangled partner in the
     * specified direction.
     * @param dir The desired direction of movement.
     * @param game A pointer to the Game instance.
     */
    void requestMove(Direction dir, Game* game) override;
};
