#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
class Game;

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
    virtual void update(const std::unique_ptr<Game> &game) = 0;
    virtual void display() const = 0;
};

class TextView : public View {
    std::vector<std::vector<std::string>> board;

   public:
    TextView(const Game &game);
    void update(const std::unique_ptr<Game> &game) override;
    void display() const override;
};

class GraphicsView : public View {
   public:
    GraphicsView(const Game &game);
    void update(const std::unique_ptr<Game> &game) override;
    void display() const override;
};
