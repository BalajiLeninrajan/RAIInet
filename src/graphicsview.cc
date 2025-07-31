#include "views.h"

#include <algorithm>
#include <iostream>
#include <string>

#include "board.h"
#include "cell.h"
#include "game.h"
#include "window.h"

GraphicsView::GraphicsView(Game *game)
    : View(game, nullptr), Xwindow(1000, 700), height(8), width(8), game{game}, cPlayer{0} {
    lm = &game->getLinkManager();
    nPlayers = game->getPlayers().size();
    players = std::vector<PlayerInfo>(nPlayers);
    b = &game->getBoard();
    boardStates = std::vector<std::vector<char>>(8, std::vector<char>(8, ' '));
    boardStates[0][3] = 'S';
    boardStates[0][4] = 'S';
    boardStates[7][3] = 'S';
    boardStates[7][4] = 'S';
    std::vector<int> cols = {lPurple, lGreen};
    for (int i=0; i<nPlayers; ++i) {
        players[i].player = game->getPlayers()[i];
        players[i].colour = cols[i];
        players[i].score = {0, 0};
        players[i].abilitiesLeft = 5;
        players[i].revealedLinks = 0;
        players[i].isAlive = true;
    }
    refresh();
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
    
    // Calculate cell size to ensure square cells, but make it smaller
    // Use the smaller dimension to ensure the grid fits, but scale it down
    int cellSize = std::min(maxGridWidth / width, maxGridHeight / height) * 2 / 3;  // Make it 2/3 of the original size
    
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

    // draw cells
    std::string firewalls = "wm";
    for (int r=0; r<8; ++r) {
        for (int c=0; c<8; ++c) {
            int col;
            if (boardStates[r][c] == 'S') {
                col = lBlue;
            } else if (firewalls.find(boardStates[r][c]) != std::string::npos) {
                col = dRed;
            } else continue;

            int x = gridX + c * cellSize + 5;
            int y = gridY + r * cellSize + 5;
            fillRectangle(x, y, cellSize-10, cellSize-10, col);
        }
    }

    
    // Draw links on the grid
    for (int playerIndex = 0; playerIndex < nPlayers; ++playerIndex) {
        const PlayerInfo& player = players[playerIndex];
        
        for (size_t linkIndex = 0; linkIndex < player.linkRepresentations.size(); ++linkIndex) {
            const auto& link = player.linkRepresentations[linkIndex];
            
            // Account for invisible rows: add 1 to row coordinate
            // Rows increase downwards, so we need to flip the calculation
            int adjustedRow = link.r - 1;
            
            // Calculate position on the grid
            int x = gridX + link.c * cellSize + 5;
            int y = gridY + adjustedRow * cellSize + 5;
            
            // Draw the main player-colored square for the link
            fillRectangle(x, y, cellSize-10, cellSize-10, player.colour);
            
            // Determine if link should be revealed
            bool isRevealed = (playerIndex == cPlayer) || 
                             (player.revealedLinks & (1 << linkIndex));
            
            // Draw the small indicator based on visibility and type
            int smallBoxSize = cellSize / 3;
            int smallBoxX = x + (cellSize - smallBoxSize) / 2;
            int smallBoxY = y + (cellSize - smallBoxSize) / 2;
            
            if (!isRevealed) {
                // Unrevealed link: draw smaller black box
                fillRectangle(smallBoxX, smallBoxY, smallBoxSize, smallBoxSize, Black);
            } else {
                // Revealed link: draw colored box based on type
                if (link.type == 'V') {
                    // Virus: draw red box
                    fillRectangle(smallBoxX, smallBoxY, smallBoxSize, smallBoxSize, dRed);
                } else if (link.type == 'D') {
                    // Data: draw green box
                    fillRectangle(smallBoxX, smallBoxY, smallBoxSize, smallBoxSize, dGreen);
                }
            }
        }
    }
}

void GraphicsView::update(View::CellUpdate update) {
    // TODO: Implement cell update logic
    // This method should update the display when a cell changes
    boardStates[update.row-1][update.col] = b->getBoard()[update.row][update.col]->cellRepresentation(game)[0];
}

void GraphicsView::update(View::RevealLinkUpdate update) {
    // TODO: Implement link reveal update logic

}

void GraphicsView::update(View::AbilityCountUpdate update) {
    // TODO: Implement ability count update logic
    // This method should update the display when ability count changes
    players[update.playerId].abilitiesLeft -= 1;
}

void GraphicsView::update(View::ScoreUpdate update) {
    // TODO: Implement score update logic
    // This method should update the display when scores change
    players[update.playerId].score = update.score;
}

void GraphicsView::drawPlayerInfo(int playerIndex, int x, int y) {
    const PlayerInfo& player = players[playerIndex];
    
    int boxWidth = 250;  // Made wider for rectangular shape
    int boxHeight = 150; // Made shorter for rectangular shape
    int margin = 10;
    int lineHeight = 15;
    
    // Draw player info box background
    fillRectangle(x, y, boxWidth, boxHeight, player.colour);
    
    // Draw border
    drawLine(x, y, x + boxWidth, y, 2, Black);
    drawLine(x, y, x, y + boxHeight, 2, Black);
    drawLine(x + boxWidth, y, x + boxWidth, y + boxHeight, 2, Black);
    drawLine(x, y + boxHeight, x + boxWidth, y + boxHeight, 2, Black);
    
    int textX = x + margin;
    int textY = y + margin + lineHeight;
    
    if (!player.isAlive) {
        // Draw DEAD text
        drawString(textX, textY, "DEAD");
        return;
    }
    
    // Draw player info
    drawString(textX, textY, "Player " + std::to_string(playerIndex + 1));
    textY += lineHeight;
    
    // Draw score
    drawString(textX, textY, "Downloaded: " + std::to_string(player.score.first) + "D, " + 
               std::to_string(player.score.second) + "V");
    textY += lineHeight;
    
    // Draw abilities
    drawString(textX, textY, "Abilities: " + std::to_string(player.abilitiesLeft));
    textY += lineHeight;
    
    // Draw links
    drawString(textX, textY, "Links:");
    textY += lineHeight;
    
    // Draw first row of links (L1 L2 L3 L4)
    std::string firstRow = "L1 L2 L3 L4: ";
    for (size_t i = 0; i < 4; ++i) {
        // Always show current player's own links, otherwise check revealed bitmask
        if (playerIndex == cPlayer || (player.revealedLinks & (1 << i))) {
            firstRow += std::string(1, player.linkRepresentations[i].type) + 
                       std::to_string(player.linkRepresentations[i].strength);
        } else {
            firstRow += "?";
        }
        if (i < 3) firstRow += " ";
    }
    drawString(textX, textY, firstRow);
    textY += lineHeight;
    
    // Draw second row of links (L5 L6 L7 L8)
    std::string secondRow = "L5 L6 L7 L8: ";
    for (size_t i = 4; i < 8; ++i) {
        // Always show current player's own links, otherwise check revealed bitmask
        if (playerIndex == cPlayer || (player.revealedLinks & (1 << i))) {
            secondRow += std::string(1, player.linkRepresentations[i].type) + 
                        std::to_string(player.linkRepresentations[i].strength);
        } else {
            secondRow += "?";
        }
        if (i < 7) secondRow += " ";
    }
    drawString(textX, textY, secondRow);
}

void GraphicsView::display() const {}

void GraphicsView::realdisplay() {
    // Call the non-const implementation
    const_cast<GraphicsView*>(this)->displayImpl();
}

void GraphicsView::refresh() {
    for (int i=0; i<nPlayers; ++i) {
        players[i].linkRepresentations = {};
        for (unsigned j=0; j<8; ++j) {
            LinkManager::LinkKey k = {
                players[i].player,
                j
            };
            if (!lm->hasLink(k)) continue;
            auto &link = lm->getLink(k);
            auto [r, c] = link.getCoords();
            players[i].linkRepresentations.push_back(linkDat{link.getStrength(), 'V', "", r, c});
            if (link.getType() == Link::LinkType::DATA) {
                players[i].linkRepresentations[players[i].linkRepresentations.size()-1].type = 'D';
            }

        }
    }
    
    // Ensure current player can see all their own links
    updateCurrentPlayerRevealedLinks();
}

void GraphicsView::nextTurn() {
    cPlayer = (cPlayer + 1) % nPlayers;
    Xwindow::display();
}

void GraphicsView::updateCurrentPlayerRevealedLinks() {
    // Set all bits for the current player's revealedLinks
    players[cPlayer].revealedLinks = 0xFF; // All 8 bits set to 1
}

void GraphicsView::displayImpl() {
    // Clear the window first
    fillRectangle(0, 0, getWidth(), getHeight(), White);
    
    // Draw the board
    drawBoard();
    
    // Draw a title
    drawString(getWidth() / 2 - 100, 30, "RAII Net");
    
    // Draw player info boxes on the right side
    int infoX = getWidth() - 270;  // 250 width + 20 margin
    int player1Y = 80;
    
    // Draw player info for each player
    for (int i = 0; i < nPlayers; ++i) {
        int y = player1Y + i * (150 + 20); // 150 height + 20 margin
        drawPlayerInfo(i, infoX, y);
    }
    
    // Update the display
    Xwindow::display();
}


