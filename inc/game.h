#pragma once

#include <memory>
#include <string>
#include <vector>

#include "link.h"

class Player;
class LinkManager;
class Board;

// Manages the overall game state, players, and turn logic.
class Game {
    std::vector<std::unique_ptr<Player>> players;
    int currentPlayerIndex;
    std::unique_ptr<Board> board;
    std::shared_ptr<LinkManager> linkManager;

   public:
    Game();
    ~Game();

    void startGame(unsigned nPlayers, const std::vector<std::string>& abilities,
                   const std::vector<std::vector<std::string>>& linkPlacements);

    void nextTurn();
    Board& getBoard();
    Player* checkWinLoss();
    Player* getCurrentPlayer();
    std::vector<Player*> getPlayers();
    int getPlayerIndex(const Player& player) const;

    const std::pair<Link::LinkType, int> getPlayerLink(
        const int playerId, const unsigned linkId) const;

    void makeMove(unsigned link, char dir);
    std::vector<std::string> showCurrentPlayerAbility();
    void useAbility(int id, const std::vector<std::string>& params);
    void printGameInfo();  // for debugging with no view
};
