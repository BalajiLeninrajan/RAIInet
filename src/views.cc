#include "views.h"

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "board.h"
#include "cell.h"
#include "game.h"
#include "link.h"
#include "window.h"

View::View(const Game *game, const Player *viewer)
    : players(), viewer(viewer), game(game) {
    for (unsigned id = 0; id < game->getPlayers().size(); ++id) {
        players.push_back({id, 5, {0, 0}});
    }
}

View::~View() {}

void View::display() const {}

char View::findBase(int index) {
    switch (index) {
        case 0:
            return 'a';
        case 1:
            return 'A';
        // case 2 and 3 are not implemented yet
        case 2:
            return 'h';
        case 3:
            return 'H';
        default:
            throw std::invalid_argument("not a valid index");
    }
}

TextView::TextView(const Game *game, const Player *viewer)
    : View(game, viewer),
      board(game->getBoard().getBoard().size(),
            std::vector<std::string>(game->getBoard().getBoard()[0].size())) {
    for (unsigned i = 0; i < board.size(); ++i) {
        for (unsigned j = 0; j < board[0].size(); ++j) {
            board[i][j] =
                game->getBoard().getBoard()[i][j]->cellRepresentation(game);
        }
    }

    for (auto &player : players) {
        char base = findBase(player.id);
        if (game->getPlayerIndex(*viewer) != player.id) {
            for (int i = 0; i < 8; ++i) {
                player.links[std::string(1, base + i)] = " ?";
            }
            continue;
        }

        for (int i = 0; i < 8; ++i) {
            const auto &link = game->getPlayerLink(player.id, i);
            player.links[std::string(1, base + i)] =
                (link.first == Link::LinkType::DATA ? "D" : "V") +
                std::to_string(link.second);
        }
    }
}

void TextView::update(View::CellUpdate update) {
    BaseCell &cell = game->getBoard().getCell({update.row, update.col});
    board[update.row][update.col] = cell.cellRepresentation(game);
}

void TextView::update(View::RevealLinkUpdate update) {
    char base = findBase(update.playerId);
    players[update.playerId].links[std::string(1, base + update.linkId)] =
        update.value;
}

void TextView::update(View::AbilityCountUpdate update) {
    players[update.playerId].abilities = update.abilityCount;
}

void TextView::update(View::ScoreUpdate update) {
    players[update.playerId].score = update.score;
}

void TextView::printPlayer(PlayerStats player) const {
    std::cout << "Player " << player.id + 1 << ":" << std::endl;
    std::cout << "Downloaded: " << player.score.first << ", "
              << player.score.second << std::endl;
    std::cout << "Abilities: " << player.abilities << std::endl;
    for (auto link : player.links) {
        std::cout << link.first << ": " << link.second;
        std::cout << " ";
    }
    std::cout << std::endl;
}

void TextView::display() const {
    // print other players
    for (auto player : players) {
        if (player.id != game->getPlayerIndex(*viewer)) {
            printPlayer(player);
        }
    }
    // print the board
    for (auto line : board) {
        for (auto tile : line) {
            std::cout << tile;
        }
        std::cout << std::endl;
    }
    printPlayer(players[game->getPlayerIndex(*viewer)]);
}

// TODO: change height of window so it's not so sadly square
GraphicsView::GraphicsView(const Game *game, const Player *viewer)
    : View(game, viewer),
      window{600, 400},
      height(game->getBoard().getBoard().size()),
      width(game->getBoard().getBoard()[0].size()) {
    for (unsigned y = 0; y < height; ++y) {
        for (unsigned x = 0; x < width; ++x) {
            std::string cell =
                game->getBoard().getBoard()[y][x]->cellRepresentation(game);
            drawCell({y, x}, cell[0]);
        }
    }
}

void GraphicsView::drawCell(std::pair<int, int> coords, char cell) {
    auto colour = Xwindow::White;
    std::string link = "";
    switch (cell) {
        case '.':
            colour = Xwindow::White;
            break;
        case 'm':
            colour = Xwindow::lRed;
            break;
        case 'w':
            colour = Xwindow::lGreen;
            break;
        // TODO: change 3 and 5 to whatever player 3 and 4 firewall are
        case '3':
            colour = Xwindow::lBlue;
            break;
        case '5':
            colour = Xwindow::lPurple;
        default:
            link += cell;
            if ('a' <= cell && cell <= 'h') {
                colour = Xwindow::dRed;
            } else if ('A' <= cell && cell < 'H') {
                colour = Xwindow::dGreen;
            } else if ('i' <= cell && cell <= 'i' + 7) {
                colour = Xwindow::dBlue;
            } else if ('I' <= cell && cell <= 'I' + 7) {
                colour = Xwindow::dPurple;
            } else {
                link = "?";  // NOTE: This should not happen
            }
    }
    // draw the rectangle here
    // if link != "" -> draw the letter too
    // TODO: check math
    int screen_width_px = static_cast<int>(window.getWidth() * 0.9 / 2);
    int screen_height_px = screen_width_px * height / width;
    int x = (window.getWidth() - screen_width_px) / 2;
    int y = (window.getHeight() - screen_height_px) / 2;
    int cell_width = screen_width_px / width;

    window.fillRectangle(x, y, cell_width, cell_width, colour);
}

void GraphicsView::update(View::CellUpdate update) {
    // TODO: implement
}
void GraphicsView::update(View::RevealLinkUpdate update) {
    // TODO: implement
}
void GraphicsView::update(View::AbilityCountUpdate update) {
    // TODO: implement
}

void GraphicsView::update(View::ScoreUpdate update) {
    // TODO: implement
}

void GraphicsView::display() const {
    // Does nothing
}
