#pragma once

#include <memory>
#include <vector>

class BaseCell;
class LinkManager;
class Link;

// Manages the game board, which is a grid of BaseCells.
class Board {
    std::vector<std::vector<std::unique_ptr<BaseCell>>> board;
    std::shared_ptr<LinkManager> linkManager;

   public:
    Board(int width, int height, std::shared_ptr<LinkManager> lm);
    ~Board();
    void moveLink(std::pair<int, int> old_coords,
                  std::pair<int, int> new_coords);
};
