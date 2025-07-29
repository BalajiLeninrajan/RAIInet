#include "board.h"

#include <memory>
#include <stdexcept>
#include <vector>

#include "cell.h"
#include "link.h"
#include "linkmanager.h"

Board::Board(int width, int height, std::shared_ptr<LinkManager> linkManager)
    : board(height), linkManager(linkManager) {
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            board[r].push_back(std::make_unique<BoardCell>(linkManager));
        }
    }
}

Board::~Board() {}

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
//
void Board::moveLink(std::pair<int, int> old_coords,
                     std::pair<int, int> new_coords) {
    if (new_coords.first < 0 || new_coords.first > (int)board.size()) {
        throw std::out_of_range("Move is out of bounds");
    }
    if (new_coords.second < 0 ||
        new_coords.second > (int)board[new_coords.first].size()) {
        throw std::out_of_range("Move is out of bounds");
    }

    LinkManager::LinkKey link =
        board[old_coords.first][old_coords.second]->getOccupantLink();

    board[new_coords.first][new_coords.second]->onEnter(link);

    linkManager->getLink(link).setCoords(new_coords);

    board[old_coords.first][old_coords.second]->emptyCell();
}

std::vector<std::vector<std::unique_ptr<BaseCell>>>& Board::getBoard() {
    return board;
}

BaseCell& Board::getCell(std::pair<int, int> coords) {
    return *board[coords.first][coords.second];
}
