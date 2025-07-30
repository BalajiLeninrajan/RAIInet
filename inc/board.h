#pragma once

#include <memory>
#include <vector>

class BaseCell;
class Player;
class Game;

// Manages the game board, which is a grid of BaseCells.
class Board {
    std::vector<std::vector<std::unique_ptr<BaseCell>>> board;
    unsigned rows;
    unsigned cols;

   public:
    Board(unsigned row, unsigned cols);
    ~Board();
    void moveLink(std::pair<int, int> old_coords,
                  std::pair<int, int> new_coords, Game* game);

    std::vector<std::vector<std::unique_ptr<BaseCell>>>& getBoard();
    BaseCell& getCell(std::pair<int, int> coords);
    void placePlayerCells(const std::vector<std::pair<int, int>> placements,
                          Player* player, unsigned row, Game* game);
};
