#pragma once

#include <memory>
#include <vector>

class BaseCell;
class Link;

// Manages the game board, which is a grid of BaseCells.
class Board {
    std::vector<std::vector<std::unique_ptr<BaseCell>>> board;

   public:
    Board(int width, int height);
    ~Board();
    bool moveLink(std::pair<int, int> old_coords,
                  std::pair<int, int> new_coords);
};
