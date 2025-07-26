#pragma once

#include "board.h"
#include "player.h"
#include <memory>
#include <string>
#include <vector>

// Manages the overall game state, players, and turn logic.
class Game {
  std::vector<Player> players;
  int currentPlayerIndex;
  std::unique_ptr<Board> board; // Game owns the board.

public:
  Game();

  void startGame();
  void nextTurn();
  Board *getBoard() { return board.get(); } // Non-owning pointer for access.
  bool checkWinLoss();                      // [cite: 11]
  Player *getCurrentPlayer();
  int getPlayerIndex(const Player *player);

  void makeMove(char link, char dir); //
  void showCurrentPlayerAbility();
  void useAbility(int id, const std::vector<std::string> &params);
};
