// views.h
#pragma once

#include <map>
#include <string>
#include <utility>  // For std::pair
#include <vector>

#include "linkmanager.h"
#include "window.h"

class Game;
class Player;
class BaseCell;

/**
 * @brief Struct to hold relevant statistics for a player for display purposes.
 */
struct PlayerStats {
    unsigned id;   /**< Player's unique ID. */
    int abilities; /**< Number of abilities remaining for the player. */
    std::pair<int, int> score; /**< Player's data and virus score. */
    std::map<std::string, std::string>
        links; /**< Map of link IDs to their string representations for display.
                */
};

/**
 * @brief Abstract base class for all game views (Observer interface).
 *
 * Views are responsible for displaying the game state to the user.
 * They observe the Game model and update their display in response to changes.
 */
class View {
   protected:
    std::vector<PlayerStats>
        players; /**< Vector of PlayerStats for all players, for displaying
                    overall game info. */
    const Player *viewer; /**< Pointer to the Player whose perspective this view
                             represents. */
    const Game *game; /**< Pointer to the Game model, allowing views to query
                         game state. */

   public:
    /**
     * @brief Struct for cell-related updates.
     *
     * Indicates that a specific cell on the board has changed and needs to be
     * redrawn.
     */
    struct CellUpdate {
        int row; /**< The row coordinate of the updated cell. */
        int col; /**< The column coordinate of the updated cell. */
    };

    /**
     * @brief Struct for revealing link information.
     *
     * Used when a link's hidden state changes or its details need to be shown.
     */
    struct RevealLinkUpdate {
        unsigned
            playerId; /**< The ID of the player who owns the revealed link. */
        unsigned linkId;   /**< The ID of the revealed link. */
        std::string value; /**< The string representation of the revealed link
                              (e.g., "V1", "D2"). */
    };

    /**
     * @brief Struct for updating ability counts.
     *
     * Notifies views when a player's available ability count changes.
     */
    struct AbilityCountUpdate {
        unsigned
            playerId; /**< The ID of the player whose ability count changed. */
        unsigned
            abilityCount; /**< The new count of abilities for the player. */
    };

    /**
     * @brief Struct for updating player scores.
     *
     * Notifies views when a player's score changes.
     */
    struct ScoreUpdate {
        unsigned playerId; /**< The ID of the player whose score changed. */
        std::pair<int, int> score; /**< The player's new score (data, virus). */
    };

    /**
     * @brief Constructor for the View class.
     * @param game A const pointer to the Game model.
     * @param viewer A const pointer to the Player whose perspective this view
     * will display.
     */
    View(const Game *game, const Player *viewer);

    /**
     * @brief Static helper to find a base character representation for a player
     * based on their index.
     * @param index The player's numerical index.
     * @return A character representing the base for that player (e.g., 'A',
     * 'B').
     */
    static char findBase(int index);

    /**
     * @brief Virtual destructor for the View class.
     */
    virtual ~View();

    /**
     * @brief Pure virtual function to update the view based on a cell change.
     * @param update A CellUpdate struct containing information about the
     * changed cell.
     */
    virtual void update(CellUpdate update) = 0;

    /**
     * @brief Pure virtual function to update the view when a link is revealed.
     * @param update A RevealLinkUpdate struct containing information about the
     * revealed link.
     */
    virtual void update(RevealLinkUpdate update) = 0;

    /**
     * @brief Pure virtual function to update the view when an ability count
     * changes.
     * @param update An AbilityCountUpdate struct containing information about
     * the ability count change.
     */
    virtual void update(AbilityCountUpdate update) = 0;

    /**
     * @brief Pure virtual function to update the view when a player's score
     * changes.
     * @param update A ScoreUpdate struct containing information about the score
     * change.
     */
    virtual void update(ScoreUpdate update) = 0;

    /**
     * @brief Pure virtual function to display the current state of the view.
     */
    virtual void display() const = 0;
};

/**
 * @brief Concrete implementation of View for console-based text display.
 *
 * This view renders the game board and player statistics as text in the
 * console.
 */
class TextView : public View {
    std::vector<std::vector<std::string>>
        board; /**< 2D vector of strings representing the textual game board. */

    /**
     * @brief Sets the string representation of a specific coordinate on the
     * textual board.
     * @param coords The (row, column) coordinates to update.
     */
    void setCoords(std::pair<int, int> coords);

    /**
     * @brief Prints a player's statistics to the console.
     * @param player A PlayerStats struct containing the player's statistics.
     */
    void printPlayer(PlayerStats player) const;

   public:
    /**
     * @brief Constructor for TextView.
     * @param game A const pointer to the Game model.
     * @param viewer A const pointer to the Player whose perspective this view
     * will display.
     */
    TextView(const Game *game, const Player *viewer);

    /**
     * @brief Updates the TextView based on a cell change.
     * @param update A CellUpdate struct.
     */
    void update(CellUpdate update) override;

    /**
     * @brief Updates the TextView when a link is revealed.
     * @param update A RevealLinkUpdate struct.
     */
    void update(RevealLinkUpdate update) override;

    /**
     * @brief Updates the TextView when an ability count changes.
     * @param update An AbilityCountUpdate struct.
     */
    void update(AbilityCountUpdate update) override;

    /**
     * @brief Updates the TextView when a player's score changes.
     * @param update A ScoreUpdate struct.
     */
    void update(ScoreUpdate update) override;

    /**
     * @brief Displays the textual game board and player information to the
     * console.
     */
    void display() const override;
};

/**
 * @brief Concrete implementation of View for graphical display
 *
 * This class is intended to provide a graphical representation of the game,
 * but its methods are currently placeholders.
 */
class GraphicsView : public View, public Xwindow {
    unsigned height;
    unsigned width;

    LinkManager *lm;
    Board *b;
    Game *game;
    std::vector<std::vector<char>> boardStates;

    struct linkDat {
        int strength;
        char type;
        std::string suffix;
        int r;
        int c;
    };

    struct PlayerInfo {
        bool isAlive;
        unsigned int revealedLinks;  // bitmask for revealed links
        std::vector<linkDat> linkRepresentations;
        unsigned int abilitiesLeft;
        std::pair<int, int> score;
        int colour;
        Player *player;
    };

    int cPlayer;

    int nPlayers;
    std::vector<PlayerInfo> players;

    void drawBoard();
    void drawPlayerInfo(int playerIndex, int x, int y);
    void updateCurrentPlayerRevealedLinks();
    void displayImpl();

   public:
    /**
     * @brief Constructor for GraphicsView.
     * @param game A const pointer to the Game model.
     * will display.
     */
    GraphicsView(Game *game);
    void refresh();
    void update(CellUpdate update) override;

    /**
     * @brief Placeholder update for revealing links in the graphical view.
     * @param update A RevealLinkUpdate struct.
     */
    void update(RevealLinkUpdate update) override;

    /**
     * @brief Placeholder update for ability count changes in the graphical
     * view.
     * @param update An AbilityCountUpdate struct.
     */
    void update(AbilityCountUpdate update) override;

    /**
     * @brief Placeholder update for score changes in the graphical view.
     * @param update A ScoreUpdate struct.
     */
    void update(ScoreUpdate update) override;

    void realdisplay();
};
