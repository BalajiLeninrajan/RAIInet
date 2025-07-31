// player.h
#pragma once

#include <memory>
#include <utility>  // For std::pair
#include <vector>

#include "linkmanager.h"

// Forward declarations to avoid circular includes
class Ability;
class Game;

/**
 * @brief Represents a player in the game.
 *
 * The Player class manages a player's score, available abilities,
 * and interactions with the LinkManager.
 */
class Player {
    std::pair<int, int>
        score; /**< Current score of the player: {data score, virus score}. */
    std::vector<std::unique_ptr<Ability>>
        abilities;     /**< Vector of unique_ptr to Ability objects the player
                          possesses. */
    int abilitiesUsed; /**< Counter for abilities used by the player in the
                          current turn/game. */
    std::shared_ptr<LinkManager>
        linkManager; /**< A shared_ptr to the LinkManager, allowing player to
                        interact with links. */

    Game* game;

   public:
    /**
     * @brief Constructor for the Player class.
     * @param abilities A vector of unique_ptr to Ability objects for this
     * player.
     * @param lm A shared_ptr to the LinkManager instance.
     */
    Player(std::vector<std::unique_ptr<Ability>> abilities,
           std::shared_ptr<LinkManager> lm, Game* game);

    /**
     * @brief Gets the player's current score.
     * @return A pair of integers representing {data score, virus score}.
     */
    std::pair<int, int> getScore() const;

    /**
     * @brief Sets the player's score to a new value.
     * @param newScore The new score as a pair of {data score, virus score}.
     */
    void setScore(std::pair<int, int> newScore);

    /**
     * @brief Gets the count of abilities used by the player.
     * @return The integer count of abilities used.
     */
    int getAbilitiesUsed() const;

    /**
     * @brief Increments the counter for abilities used by the player.
     */
    void incrementAbilityUse();

    /**
     * @brief Gets a constant reference to the player's abilities.
     * @return A const reference to the vector of unique_ptr to Ability.
     */
    const std::vector<std::unique_ptr<Ability>> &getAbilities() const;

    /**
     * @brief Simulates a download action for a given link, typically leading to
     * scoring.
     * @param linkKey The LinkManager::LinkKey of the link to be downloaded.
     */
    void download(LinkManager::LinkKey linkKey);
};
