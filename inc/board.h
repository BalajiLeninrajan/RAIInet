#pragma once

#include <memory>
#include <vector>

#include "cell.h"

class Link;

// Manages the game board, which is a grid of BaseCells.
class Board {
    std::vector<std::vector<std::unique_ptr<BaseCell>>> board;

   public:
    Board(int width, int height);
    ~Board();
    bool moveLink(Link &link, std::pair<int, int> coords);
};
