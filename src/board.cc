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
bool Board::moveLink(Link& link, std::pair<int, int> coords) {
    // implement
    // TODO: implement later
    // assuming coords = [x,y]
    // checks x and y coords in range
    if (coords.second < 0 || coords.second > (int)board.size()) {
        throw std::out_of_range("Move is out of bounds");
    } else if (coords.first < 0 ||
               coords.first > (int)board[coords.second].size()) {
        throw std::out_of_range("Move is out of bounds");
    }
    try {
        auto link_ptr = std::make_shared<Link>(link);
        board[coords.second][coords.first]->onEnter(link_ptr);
        link.setCoords(coords);
    } catch (const std::invalid_argument& e) {
        throw;
    }
    return true;
}
