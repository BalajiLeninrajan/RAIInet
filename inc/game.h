#pragma once

#include <memory>
#include <string>
#include <vector>

class Player;
class Board;

// Manages the overall game state, players, and turn logic.
class Game {
    std::vector<std::unique_ptr<Player>> players;
    int currentPlayerIndex;
    std::unique_ptr<Board> board;

   public:
    Game();
    ~Game();

    void startGame();
    void nextTurn();
    Board& getBoard();
    bool checkWinLoss();
    Player* getCurrentPlayer();
    int getPlayerIndex(const Player& player);

    void makeMove(char link, char dir);
    void showCurrentPlayerAbility();
    void useAbility(int id, const std::vector<std::string>& params);
};
