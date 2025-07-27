#pragma once

#include <memory>
#include <string>
#include <vector>

#include "board.h"
#include "player.h"

// Manages the overall game state, players, and turn logic.
class Game {
    std::vector<Player> players;
    int currentPlayerIndex;
    std::unique_ptr<Board> board;

   public:
    Game();
    ~Game();

    void startGame(int nPlayers, const std::vector<std::string> &abilities, const std::vector<std::vector<std::string>> &linkPlacements);

    void nextTurn();
    std::weak_ptr<Board> getBoard();
    bool checkWinLoss();
    std::weak_ptr<Player> getCurrentPlayer();
    int getPlayerIndex(const std::weak_ptr<Player>& player);

    void makeMove(char link, char dir);
    void showCurrentPlayerAbility();
    void useAbility(int id, const std::vector<std::string>& params);
};
