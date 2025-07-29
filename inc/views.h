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
    std::vector<PlayerStats> players;

   public:
    virtual ~View();
    virtual void update(std::unique_ptr<BaseCell> oldCell,
                        std::unique_ptr<BaseCell> newCell) = 0;
    virtual void display() const = 0;
};

class TextView : public View {
    std::vector<std::vector<std::string>> board;

   public:
    TextView(const std::unique_ptr<Game> &game);
    void update(std::unique_ptr<BaseCell> oldCell,
                std::unique_ptr<BaseCell> newCell) override;
    void display() const override;
};

class GraphicsView : public View {
   public:
    GraphicsView(const std::unique_ptr<Game> &game);
    void update(std::unique_ptr<BaseCell> oldCell,
                std::unique_ptr<BaseCell> newCell) override;
    void display() const override;
};
