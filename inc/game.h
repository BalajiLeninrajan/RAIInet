#pragma once

#include <memory>
#include <queue>
#include <string>
#include <variant>
#include <vector>

#include "link.h"
#include "views.h"

class Player;
class LinkManager;
class Board;

// Manages the overall game state, players, and turn logic.
class Game {
    typedef std::variant<View::AbilityCountUpdate, View::CellUpdate,
                         View::RevealLinkUpdate, View::ScoreUpdate>
        update_type;
    std::vector<std::unique_ptr<Player>> players;
    int currentPlayerIndex;
    std::unique_ptr<Board> board;
    std::shared_ptr<LinkManager> linkManager;
    std::queue<update_type> queue;

   public:
    Game();
    ~Game();

    void startGame(unsigned nPlayers, const std::vector<std::string>& abilities,
                   const std::vector<std::vector<std::string>>& linkPlacements);

    void nextTurn();
    Board& getBoard() const;
    Player* checkWinLoss();
    Player* getCurrentPlayer();
    std::vector<Player*> getPlayers() const;
    unsigned getPlayerIndex(const Player& player) const;
    LinkManager& getLinkManager() const;

    void addUpdate(update_type update);
    std::queue<update_type> flushUpdates();

    const std::pair<Link::LinkType, int> getPlayerLink(
        const int playerId, const unsigned linkId) const;

    void makeMove(unsigned link, char dir);
    std::vector<std::string> showCurrentPlayerAbility();
    void useAbility(int id, const std::vector<std::string>& params);
    void printGameInfo();  // for debugging with no view
};
