#pragma once

#include <map>
#include <memory>
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
    View(const std::unique_ptr<Game> &game);
    virtual ~View();
    virtual void update(std::pair<int, int> old_coords,
                        std::pair<int, int> new_coords) = 0;
    virtual void display() const = 0;
};

class TextView : public View {
    std::vector<std::vector<std::string>> board;
    unsigned playerId;
    const std::unique_ptr<Game> &game;
    void setCoords(std::pair<int, int> coords);
    void printPlayer(PlayerStats player);

   public:
    TextView(const std::unique_ptr<Game> &game, unsigned playerId);
    void update(std::pair<int, int> old_coords,
                std::pair<int, int> new_coords) override;
    void display() const override;
};

class GraphicsView : public View {
   public:
    GraphicsView(const std::unique_ptr<Game> &game);
    void update(std::pair<int, int> old_coords,
                std::pair<int, int> new_coords) override;
    void display() const override;
};
