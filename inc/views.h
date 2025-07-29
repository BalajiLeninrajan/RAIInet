#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Game;
class BaseCell;

struct PlayerStats {
    int id;
    int abilities;
    std::pair<int, int> score;
    std::unordered_map<std::string, std::string> links;
};

class View {
   protected:
    std::vector<PlayerStats> players;

   public:
    virtual ~View();
    virtual void update(std::pair<int, int> old_coords,
                        std::pair<int, int> new_coords) = 0;
    virtual void display() const = 0;
};

class TextView : public View {
    std::vector<std::vector<std::string>> board;
    int playerId;

   public:
    TextView(const std::unique_ptr<Game> &game, int playerId);
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
