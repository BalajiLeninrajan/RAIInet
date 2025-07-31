// linkmanager.h
#pragma once
#include <functional>  // For std::function
#include <map>
#include <memory>
#include <string>
#include <vector>

class Player;
class Link;
class Board;

/**
 * @brief Manages all active links in the game, associating them with their
 * owners.
 *
 * The LinkManager provides a centralized way to access, add, remove, and modify
 * links, indexed by their owner and a unique ID.
 */
class LinkManager {
    std::map<Player*, std::vector<std::unique_ptr<Link>>>
        linkMap; /**< Map from Player* to a vector of unique_ptr<Link>, storing
                    each player's links. */

   public:
    /**
     * @brief Struct to uniquely identify a link by its owner and ID.
     */
    struct LinkKey {
        Player* player; /**< Pointer to the owning player. */
        unsigned id;    /**< The unique ID of the link within that player's
                           collection. */

        /**
         * @brief Comparison operator for LinkKey, enabling its use as a map key
         * or in sets.
         * @param other The other LinkKey to compare against.
         * @return True if this LinkKey is less than the other (for ordering
         * purposes).
         */
        bool operator<(const LinkKey& other) const {
            if (player != other.player) {
                return player < other.player;
            }
            return id < other.id;
        }
    };

    /**
     * @brief Constructor for LinkManager.
     */
    LinkManager();

    /**
     * @brief Adds a set of links for a given player.
     * @param links A vector of strings, each representing a link to be added
     * (e.g., "v1", "d2").
     * @param player A pointer to the Player who will own these links.
     * @param board A pointer to the game Board where the links will be placed.
     */
    void addLinksForPlayer(const std::vector<std::string>& links,
                           Player* player, Board* board);

    /**
     * @brief Removes a specific link identified by its LinkKey.
     * @param key The LinkKey of the link to be removed.
     * @return True if the link was found and removed, false otherwise.
     */
    bool removeLink(LinkKey key);

    /**
     * @brief Checks if a link identified by its LinkKey exists in the manager.
     * @param key The LinkKey of the link to check.
     * @return True if the link exists, false otherwise.
     */
    bool hasLink(LinkKey key);

    /**
     * @brief Gets a reference to a link identified by its LinkKey.
     * @param key The LinkKey of the link to retrieve.
     * @return A reference to the Link object.
     * @throws std::out_of_range If the link is not found.
     */
    Link& getLink(LinkKey key);

    /**
     * @brief Cleans up all links associated with a specified player.
     *
     * This is typically called when a player is eliminated from the game.
     *
     * @param p A pointer to the Player whose links are to be cleaned up.
     * @return True if the player existed in the link manager and their links
     * were processed, false otherwise.
     */
    bool cleanPlayer(Player* p);

    /**
     * @brief Checks if a player has no remaining links.
     * @param p A pointer to the Player to check.
     * @return True if the player has no links, false otherwise.
     */
    bool playerIsEmpty(Player* p);

    /**
     * @brief Applies a decorator function to a specific link.
     *
     * This function takes a `std::function` that transforms a
     * `unique_ptr<Link>` into another `unique_ptr<Link>` (the decorated
     * version) and applies it to the link identified by `key`.
     *
     * @param key The LinkKey of the link to decorate.
     * @param decorator A `std::function` that takes a `unique_ptr<Link>` and
     * returns a `unique_ptr<Link>` (the decorated link).
     * @return True if the decorator was successfully applied, false if the link
     * was not found.
     */
    bool applyDecorator(
        LinkKey key,
        std::function<std::unique_ptr<Link>(std::unique_ptr<Link>)>& decorator);
};
