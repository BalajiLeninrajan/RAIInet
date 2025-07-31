// game.h
#pragma once

#include <memory>
#include <queue>
#include <string>
#include <variant>
#include <vector>

#include "link.h"
#include "views.h"

class Player;
class LinkManager;
class Board;

/**
 * @brief Manages the overall game state, players, and turn logic (the Model in
 * MVC).
 *
 * The Game class is the central component that encapsulates the game's rules,
 * manages players, the board, links, and orchestrates actions like moves and
 * ability usage.
 */
class Game {
    /**
     * @brief A type alias for a variant holding different update types for
     * views.
     *
     * This allows the Game to send various kinds of state changes to observers
     * (Views) through a single mechanism.
     */
    typedef std::variant<View::AbilityCountUpdate, View::CellUpdate,
                         View::RevealLinkUpdate, View::ScoreUpdate>
        update_type;

    std::vector<std::unique_ptr<Player>>
        players; /**< Vector of unique_ptr to Player objects participating in
                    the game. */
    int currentPlayerIndex; /**< Index of the currently active player in the
                               `players` vector. */
    std::unique_ptr<Board>
        board; /**< A unique_ptr to the game Board instance. */
    std::shared_ptr<LinkManager>
        linkManager; /**< A shared_ptr to the LinkManager instance, managing all
                        links. */
    std::queue<update_type> queue; /**< A queue of update_type variants for view
                                      updates (Observer pattern). */

   public:
    /**
     * @brief Constructor for the Game class.
     */
    Game();

    /**
     * @brief Destructor for the Game class.
     */
    ~Game();

    /**
     * @brief Initializes and starts a new game with the specified number of
     * players, abilities, and link placements.
     * @param nPlayers The number of players in the game.
     * @param abilities A vector of strings representing the abilities available
     * to players.
     * @param linkPlacements A vector of vectors of strings, detailing the
     * initial placement of links for each player.
     */
    void startGame(unsigned nPlayers, const std::vector<std::string>& abilities,
                   const std::vector<std::vector<std::string>>& linkPlacements);

    /**
     * @brief Advances the game to the next player's turn.
     */
    void nextTurn();

    /**
     * @brief Gets a reference to the game board.
     * @return A reference to the Board instance.
     */
    Board& getBoard() const;

    /**
     * @brief Cleans up players who have lost all their links, effectively
     * removing them from the game.
     */
    void cleanPlayers();

    /**
     * @brief Checks for win/loss conditions in the game.
     * @return A pointer to the winning Player, or nullptr if no player has won
     * yet.
     */
    Player* checkWinLoss();

    /**
     * @brief Gets a pointer to the currently active player.
     * @return A pointer to the current Player.
     */
    Player* getCurrentPlayer();

    /**
     * @brief Gets a vector of pointers to all players in the game.
     * @return A vector of Player pointers.
     */
    std::vector<Player*> getPlayers() const;

    /**
     * @brief Gets the index of a given player in the internal players vector.
     * @param player A const reference to the Player whose index is to be found.
     * @return The unsigned integer index of the player.
     */
    unsigned getPlayerIndex(const Player& player) const;

    /**
     * @brief Gets a reference to the LinkManager instance.
     * @return A reference to the LinkManager.
     */
    LinkManager& getLinkManager() const;

    /**
     * @brief Adds a view update to the internal update queue.
     *
     * Views will retrieve these updates to refresh their display.
     *
     * @param update The update_type variant containing the specific update
     * information.
     */
    void addUpdate(update_type update);

    /**
     * @brief Flushes (empties) the internal update queue and returns its
     * contents.
     * @return A queue of update_type variants representing all accumulated
     * updates.
     */
    std::queue<update_type> flushUpdates();

    /**
     * @brief Retrieves the LinkType and strength of a specific player's link.
     * @param playerId The ID of the player.
     * @param linkId The ID of the link owned by the player.
     * @return A pair where the first element is the LinkType and the second is
     * its strength.
     */
    const std::pair<Link::LinkType, int> getPlayerLink(
        const int playerId, const unsigned linkId) const;

    /**
     * @brief Executes a movement action for a specified link in a given
     * direction.
     * @param link The ID of the link to move.
     * @param dir The character representing the direction of movement ('N',
     * 'S', 'E', 'W').
     */
    void makeMove(unsigned link, char dir);

    /**
     * @brief Activates a player's ability.
     * @param id The ID of the ability to use.
     * @param params A vector of strings containing any parameters required for
     * the ability.
     */
    void useAbility(int id, const std::vector<std::string>& params);

    /**
     * @brief Prints current game information (e.g., player scores, active
     * links) for debugging or status display.
     */
    void printGameInfo();
};
