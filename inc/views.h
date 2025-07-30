#pragma once

#include <map>
#include <string>
#include <vector>

class Game;
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

   public:
    View(const Game *game);
    static char findBase(int index);
    virtual ~View();
    /*
     * Notify view that the coordinate [r, c] has changed.
     */
    virtual void update(std::pair<int, int> coords) = 0;
    /*
     * For revealing links:
     * Tell views to reveal playerid's link at linkid should be revealed
     * as value.
     */
    virtual void update(int playerId, int linkId, std::string value) = 0;
    virtual void display() const = 0;
};

class TextView : public View {
    std::vector<std::vector<std::string>> board;
    unsigned currentPlayer;
    const Game *game;
    void setCoords(std::pair<int, int> coords);
    void printPlayer(PlayerStats player) const;

   public:
    TextView(const Game *game, unsigned currentPlayer);
    void update(std::pair<int, int> coords) override;
    void update(int playerId, int linkId, std::string value) override;
    void display() const override;
};

class GraphicsView : public View {
   public:
    GraphicsView(const Game *game);
    void update(std::pair<int, int> coords) override;
    void display() const override;
};
