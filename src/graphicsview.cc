#include "views.h"

#include <algorithm>
#include <iostream>
#include <string>

#include "board.h"
#include "cell.h"
#include "game.h"

GraphicsView::GraphicsView(const Game *game)
    : View(game, nullptr), Xwindow(800, 600), height(8), width(8) {
    // Initialize the graphics view
}

void GraphicsView::drawBoard() {
    // Calculate grid dimensions and positioning
    int windowWidth = getWidth();
    int windowHeight = getHeight();
    
    // Leave some margin around the grid
    int margin = 50;
    
    // Calculate the maximum grid size that fits in the window with margins
    int maxGridWidth = windowWidth - 2 * margin;
    int maxGridHeight = windowHeight - 2 * margin;
    
    // Calculate cell size to ensure square cells
    // Use the smaller dimension to ensure the grid fits
    int cellSize = std::min(maxGridWidth / width, maxGridHeight / height);
    
    // Calculate actual grid dimensions
    int gridWidth = cellSize * width;
    int gridHeight = cellSize * height;
    
    // Position grid at the left edge with margin
    int gridX = margin;
    int gridY = (windowHeight - gridHeight) / 2;
    
    // Draw grid lines
    // Vertical lines
    for (unsigned col = 0; col <= width; ++col) {
        int x = gridX + col * cellSize;
        drawLine(x, gridY, x, gridY + gridHeight, 2, Black);
    }
    
    // Horizontal lines
    for (unsigned row = 0; row <= height; ++row) {
        int y = gridY + row * cellSize;
        drawLine(gridX, y, gridX + gridWidth, y, 2, Black);
    }
    
    // TODO: Draw cells in each grid cell
    // This will be implemented later when we add cell drawing logic
}

void GraphicsView::drawCell(std::pair<int, int> coords, char cell) {
    // TODO: Implement cell drawing logic
    // This method should draw a single cell at the given coordinates
    // with the appropriate color and content based on the cell character
}

void GraphicsView::update(View::CellUpdate update) {
    // TODO: Implement cell update logic
    // This method should update the display when a cell changes
}

void GraphicsView::update(View::RevealLinkUpdate update) {
    // TODO: Implement link reveal update logic
    // This method should update the display when a link is revealed
}

void GraphicsView::update(View::AbilityCountUpdate update) {
    // TODO: Implement ability count update logic
    // This method should update the display when ability count changes
}

void GraphicsView::update(View::ScoreUpdate update) {
    // TODO: Implement score update logic
    // This method should update the display when scores change
}

void GraphicsView::display() const {
    // Call the non-const implementation
    const_cast<GraphicsView*>(this)->displayImpl();
}

void GraphicsView::displayImpl() {
    // Clear the window first
    fillRectangle(0, 0, getWidth(), getHeight(), White);
    
    // Draw the board
    drawBoard();
    
    // Update the display
    Xwindow::display();
}


