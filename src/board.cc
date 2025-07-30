#include "board.h"

#include <memory>
#include <stdexcept>
#include <vector>

#include "cell.h"
#include "link.h"
#include "linkmanager.h"
#include "player.h"
#include <iostream>

Board::Board(int rows, int cols, std::shared_ptr<LinkManager> linkManager)
    : board(rows), linkManager(linkManager) {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            board[r].push_back(std::make_unique<BoardCell>(linkManager));
        }
    }
}

void Board::placePlayerCells(const std::vector<std::pair<int, int>> placements, Player* player) {
    for (int i=0; i<2; ++i) {
        auto &[r, c] = placements[i];
        std::cout << r << " " << c << std::endl;
        
        board[r][c] = std::make_unique<Server>(std::move(board[r][c]), player);
    }

    for (unsigned i=2; i<placements.size(); ++i) {
        auto &[r, c] = placements[i];
        LinkManager::LinkKey k{player, i-2};
        board[r][c]->onEnter(k);
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

PlayerCell::~PlayerCell() {}
