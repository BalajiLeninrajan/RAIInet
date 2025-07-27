#include "board.h"

#include <memory>
#include <stdexcept>
#include <vector>

#include "cell.h"
#include "link.h"

Board::Board(int width, int height) : board(height) {
    for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
            board[h].emplace_back(std::make_unique<BoardCell>());
        }
    }
}

//  Board checks co-ordinates
//  - board calls onEnter on cell
//  onEnter could throw
//  if cell empty -> update link position -> link handles
//  if cell is full -> initiates battle -> update link position
//  if link is is on one of its own -> throw

//  Responsibilities for moveLink
//  - checks if co-ords in range
//  - runs onEnter() on cell
// coords is [y,x]
bool Board::moveLink(std::pair<int, int> old_coords,
                     std::pair<int, int> new_coords) {
    if (new_coords.first < 0 || new_coords.first > (int)board.size()) {
        throw std::out_of_range("Move is out of bounds");
    } else if (new_coords.second < 0 ||
               new_coords.second > (int)board[new_coords.first].size()) {
        throw std::out_of_range("Move is out of bounds");
    }
    try {
        board[new_coords.first][new_coords.second]->onEnter(
            board[old_coords.first][old_coords.second]->getOccupantLink());
        board[old_coords.first][old_coords.second]
            ->getOccupantLink()
            ->setCoords(new_coords);
    } catch (const std::invalid_argument& e) {
        throw;
    }
    return true;
}
