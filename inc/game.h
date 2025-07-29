#pragma once

#include <memory>
#include <string>
#include <vector>

#include "linkmanager.h"

class Player;
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

    void startGame(int nPlayers, const std::vector<std::string>& abilities,
                   const std::vector<std::vector<std::string>>& linkPlacements);

    void nextTurn();
    Board& getBoard();
    Player* checkWinLoss();
    Player* getCurrentPlayer();
    std::vector<Player*> getPlayers();
    int getPlayerIndex(const Player& player);

    const std::vector<Link&> getPlayerLinks(const Player& player);

    void makeMove(int link, char dir);
    std::vector<std::string> showCurrentPlayerAbility();
    void useAbility(int id, const std::vector<std::string>& params);
};
