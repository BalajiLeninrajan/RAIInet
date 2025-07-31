// board.h
#pragma once

#include <memory>
#include <utility>
#include <vector>

class BaseCell;
class Player;
class Game;

/**
 * @brief Manages the game board, which is a grid of BaseCell objects.
 *
 * The Board class is responsible for the layout and manipulation of cells,
 * including moving links between cells and placing/removing player-specific
 * cells.
 */
class Board {
    std::vector<std::vector<std::unique_ptr<BaseCell>>>
        board;     /**< 2D vector storing unique_ptr to BaseCell objects. */
    unsigned rows; /**< Number of rows on the board. */
    unsigned cols; /**< Number of columns on the board. */

   public:
    /**
     * @brief Constructor for the Board class.
     * @param row The number of rows for the board.
     * @param cols The number of columns for the board.
     */
    Board(unsigned row, unsigned cols);

    /**
     * @brief Destructor for the Board class.
     */
    ~Board();

    /**
     * @brief Moves a link from one set of coordinates to another on the board.
     * @param old_coords The original coordinates of the link.
     * @param new_coords The new coordinates for the link.
     * @param game A pointer to the Game instance, needed for cell interactions
     * (e.g., onEnter).
     */
    void moveLink(std::pair<int, int> old_coords,
                  std::pair<int, int> new_coords, Game* game);

    /**
     * @brief Gets a reference to the internal 2D vector representing the board.
     * @return A reference to the board's 2D vector of unique_ptr to BaseCell.
     */
    std::vector<std::vector<std::unique_ptr<BaseCell>>>& getBoard();

    /**
     * @brief Gets a reference to the cell at the specified coordinates.
     * @param coords The (row, column) coordinates of the cell to retrieve.
     * @return A reference to the BaseCell at the given coordinates.
     */
    BaseCell& getCell(std::pair<int, int> coords);

    /**
     * @brief Places player-specific cells (e.g., Servers, Goals) on the board.
     * @param placements A vector of coordinate pairs where cells should be
     * placed.
     * @param player A pointer to the Player who owns these cells.
     * @param row The row index where cells are being placed (for
     * player-specific row allocation).
     * @param game A pointer to the Game instance.
     */
    void placePlayerCells(const std::vector<std::pair<int, int>> placements,
                          Player* player, unsigned row, Game* game);

    /**
     * @brief Removes all cells associated with a given player from the board,
     * reverting them to BoardCells.
     * @param player A pointer to the Player whose cells are to be removed.
     */
    void removePlayerCells(Player* player);

    /**
     * @brief Undecorates a cell, reverting it to its base form (e.g., from
     * Firewall to Server).
     * @param cell A unique_ptr reference to the cell to be undecorated. The
     * unique_ptr will be reset.
     * @param p A pointer to the Player who owns the cell.
     */
    void undecorateCell(std::unique_ptr<BaseCell>& cell, Player* p);
};
