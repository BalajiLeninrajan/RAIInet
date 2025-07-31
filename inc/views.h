#pragma once

#include <map>
#include <string>
#include <vector>

#include "window.h"
#include "linkmanager.h"

class Game;
class Player;
class BaseCell;

struct PlayerStats {
    unsigned id;
    int abilities;
    std::pair<int, int> score;
    std::map<std::string, std::string> links;
};

class View {
   protected:
    std::vector<PlayerStats> players;
    const Player *viewer;
    const Game *game;

   public:
    struct CellUpdate {
        int row;
        int col;
    };

    struct RevealLinkUpdate {
        unsigned playerId;
        unsigned linkId;
        std::string value;
    };

    struct AbilityCountUpdate {
        unsigned playerId;
        unsigned abilityCount;
    };

    struct ScoreUpdate {
        unsigned playerId;
        std::pair<int, int> score;
    };

    View(const Game *game, const Player *viewer);
    static char findBase(int index);
    virtual ~View();
    /*
     * Notify view that the coordinate [r, c] has changed.
     */
    virtual void update(CellUpdate update) = 0;
    /*
     * For revealing links:
     * Tell views to reveal playerid's link at linkid should be revealed
     * as value.
     */
    virtual void update(RevealLinkUpdate update) = 0;

    /*
    * update player playerId with that they have abilityCount abilities remaining
    */
    virtual void update(AbilityCountUpdate update) = 0;

    /*
    * Update player playerId with that they have [d, v] data/viruses (score)
    */
    virtual void update(ScoreUpdate update) = 0;
    virtual void display() const = 0;
};

class TextView : public View {
    std::vector<std::vector<std::string>> board;

    void setCoords(std::pair<int, int> coords);
    void printPlayer(PlayerStats player) const;

   public:
    TextView(const Game *game, const Player *viewer);

    void update(CellUpdate update) override;
    void update(RevealLinkUpdate update) override;
    void update(AbilityCountUpdate update) override;
    void update(ScoreUpdate update) override;
    void display() const override;
};

class GraphicsView : public View, public Xwindow {
    unsigned height;
    unsigned width;

    LinkManager* lm;
    Board* b;
    Game* game;
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
        Player* player;
    };

    int cPlayer;

    int nPlayers;
    std::vector<PlayerInfo> players;


    void drawBoard();
    void drawPlayerInfo(int playerIndex, int x, int y);
    void updateCurrentPlayerRevealedLinks();
    void displayImpl();

   public:
    GraphicsView(Game *game);
    void refresh();
    void update(CellUpdate update) override;
    void update(RevealLinkUpdate update) override;
    void update(AbilityCountUpdate update) override;
    void update(ScoreUpdate update) override;
    void realdisplay();
    void display() const override;
    void nextTurn();
};
