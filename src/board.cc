#include "board.h"

#include <assert.h>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <vector>

#include "cell.h"
#include "game.h"
#include "link.h"
#include "linkmanager.h"
#include "player.h"

Board::Board(unsigned rows, unsigned cols)
    : board(rows), rows{rows}, cols{cols} {
    for (unsigned r = 0; r < rows; ++r) {
        for (unsigned c = 0; c < cols; ++c) {
            board[r].push_back(std::make_unique<BoardCell>());
        }
    }
}

void Board::placePlayerCells(const std::vector<std::pair<int, int>> placements,
                             Player* player, unsigned goalRow, Game* game) {
    for (int i = 0; i < 2; ++i) {
        auto& [r, c] = placements[i];
        board[r][c] = std::make_unique<Server>(std::move(board[r][c]), player);
    }

    for (unsigned i = 2; i < placements.size(); ++i) {
        auto& [r, c] = placements[i];
        LinkManager::LinkKey k{player, i - 2};
        game->getLinkManager().getLink(k).setCoords(placements[i]);
        board[r][c]->onEnter(k, game);
    }

    for (unsigned c = 0; c < cols; ++c) {
        board[goalRow][c] =
            std::make_unique<Goal>(std::move(board[goalRow][c]), player);
    }
}

void Board::removePlayerCells(Player *player) {
    for (unsigned r=0; r<rows; ++r) {
        for (unsigned c=0; c<cols; ++c) {
            undecorateCell(board[r][c], player);
            if (board[r][c]->isOccupied()) {
                std::cout << r << " " << c << "\n";
                auto lk = board[r][c]->getOccupantLink();
                if (lk.player == player) {
                    board[r][c]->emptyCell();
                }
            }
        }
    }
}


void Board::undecorateCell(std::unique_ptr<BaseCell> &cell, Player *p) {
    if (cell == nullptr) {
        throw std::invalid_argument("Attempted to undecorate nullptr");
    }

    if (auto playercell = dynamic_cast<PlayerCell*>(cell.get())) {
        undecorateCell(playercell->base, p);
        if (playercell->owner == p) {
            cell = std::move(playercell->base);
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
                     std::pair<int, int> new_coords, Game* game) {
    // we truncate the y-coordinate as moving past the edge is represented by
    // moving into one of the "goal" cells.
    // Relies on the assertion that a cell on the top/bottom edges is always a
    // goal.
    new_coords.first = std::min(new_coords.first, (int)rows - 1);
    new_coords.first = std::max(new_coords.first, 0);
    if (new_coords.first < 0 || new_coords.first > (int)board.size()) {
        throw std::out_of_range("Move is out of bounds");
    }
    if (new_coords.second < 0 ||
        new_coords.second > (int)board[new_coords.first].size()) {
        throw std::out_of_range("Move is out of bounds");
    }

    LinkManager::LinkKey link =
        board[old_coords.first][old_coords.second]->getOccupantLink();

    board[new_coords.first][new_coords.second]->onEnter(link, game);

    // onEnter may delete the link
    if (game->getLinkManager().hasLink(link)) {
        game->getLinkManager().getLink(link).setCoords(new_coords);
    }
    board[old_coords.first][old_coords.second]->emptyCell();
    View::CellUpdate oldCoordsUpdate{old_coords.first, old_coords.second};
    View::CellUpdate newCoordsUpdate{new_coords.first, new_coords.second};
    game->addUpdate(oldCoordsUpdate);
    game->addUpdate(newCoordsUpdate);
}
std::vector<std::vector<std::unique_ptr<BaseCell>>>& Board::getBoard() {
    return board;
}

BaseCell& Board::getCell(std::pair<int, int> coords) {
    return *board[coords.first][coords.second];
}

PlayerCell::~PlayerCell() {}
