#include "game.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <tuple>

#include "ability.h"
#include "board.h"
#include "cell.h"
#include "factories.h"
#include "link.h"
#include "linkmanager.h"
#include "player.h"

using LinkKey = LinkManager::LinkKey;
typedef std::variant<std::pair<int, int>, std::tuple<int, int, std::string>>
    update_type;

void Game::startGame(
    unsigned nPlayers, const std::vector<std::string>& abilities,
    const std::vector<std::vector<std::string>>& linkPlacements) {
    linkManager = std::make_shared<LinkManager>();
    // create board
    board = std::make_unique<Board>(10, 8);

    // create player objects
    if (abilities.size() != nPlayers || linkPlacements.size() != nPlayers) {
        throw std::invalid_argument(
            "Incorrect number of abilities/link placements.");
    }

    players.clear();

    for (unsigned i = 0; i < nPlayers; ++i) {
        std::vector<std::unique_ptr<Ability>> p_abilities;
        for (auto ch : abilities[i]) {
            p_abilities.push_back(AbilityFactory::createPlayerAbility(ch));
        }
        players.push_back(
            std::make_unique<Player>(std::move(p_abilities), linkManager));
        linkManager->addLinksForPlayer(linkPlacements[i], players[i].get(),
                                       board.get());
    }

    // for now, assume the board is 10 rows x 8 cols
    // and that the 1st and last rows are goal rows.
    // first 2 placements are server ports.
    std::vector<std::pair<int, int>> p1placements = {
        {8, 3}, {8, 4}, {8, 0}, {8, 1}, {8, 2},
        {7, 3}, {7, 4}, {8, 5}, {8, 6}, {8, 7}};

    std::vector<std::pair<int, int>> p2placements = {
        {1, 3}, {1, 4}, {1, 0}, {1, 1}, {1, 2},
        {2, 3}, {2, 4}, {1, 5}, {1, 6}, {1, 7}};

    board->placePlayerCells(p1placements, players[0].get(), 9, this);  // p1
    board->placePlayerCells(p2placements, players[1].get(), 0, this);  // p2

    currentPlayerIndex = 0;
    // printGameInfo();
}

Player* Game::getCurrentPlayer() { return players[currentPlayerIndex].get(); }

Player* Game::checkWinLoss() {
    // count players
    int activePlayerCount = 0;
    Player* p = nullptr;
    for (const auto& it : players) {
        if (it != nullptr) {
            activePlayerCount++;
            p = it.get();
            // downloaded 4 links
            // implicit assertion: only 1 player can reach 4 links before this
            // is called (does not handle winning ties)
            if (p->getScore().first >= 4) {
                return p;
            }
        }
    }

    if (activePlayerCount == 1) {
        return p;
    }
    return nullptr;
}

unsigned Game::getPlayerIndex(const Player& player) const {
    for (unsigned i = 0; i < players.size(); ++i) {
        if (players[i].get() == &player) return i;
    }
    return -1;
}

LinkManager& Game::getLinkManager() const { return *linkManager; }

void Game::nextTurn() {
    cleanPlayers();
    do {
        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    } while (players[currentPlayerIndex] == nullptr);
    std::cout << "Turn of Player " << currentPlayerIndex + 1 << "\n";
}

void Game::makeMove(unsigned link, char dir) {
    try {
        LinkKey linkKey = LinkKey{players[currentPlayerIndex].get(), link};
        linkManager->getLink(linkKey).requestMove(Link::charToDirection(dir),
                                                  this);
        nextTurn();
    } catch (std::exception& e) {
        std::cout << "Invalid command: " << e.what() << std::endl;
        // comment this out for final build
        throw e;
    }
}

void Game::addUpdate(update_type update) { queue.push(update); }

std::queue<Game::update_type> Game::flushUpdates() {
    std::queue<update_type> temp = std::move(queue);
    queue = {};
    return temp;
}

void Game::useAbility(int id, const std::vector<std::string>& params) {
    std::cout << "I'm using an ability RAAAHHHHHHH" << "\n";
}

std::vector<Player*> Game::getPlayers() const {
    std::vector<Player*> result(players.size());
    std::transform(players.begin(), players.end(), result.begin(),
                   [](const std::unique_ptr<Player>& p) { return p.get(); });
    return result;
}

void Game::cleanPlayers() {
    for (auto &pl: players) {
        if (pl != nullptr) {
            // loss condition 1: player has 4 viruses
            bool has4virus = pl->getScore().second >= 4;
            // loss condition 2: player has no links
            bool noLinks = linkManager->playerIsEmpty(pl.get());
            std::cout << has4virus << " " << "bongo2\n";
            if (has4virus || noLinks) {
                // clear board
                board->removePlayerCells(pl.get());
                // clean link manager
                linkManager->cleanPlayer(pl.get());
                // set to nullptr
                pl = nullptr;
            }

        }
    }
}

Board& Game::getBoard() const { return *board; }

const std::pair<Link::LinkType, int> Game::getPlayerLink(
    const int playerId, const unsigned linkId) const {
    LinkKey linkKey = LinkKey{players[playerId].get(), linkId};
    const Link& link = linkManager->getLink(linkKey);
    return {link.getType(), link.getStrength()};
}

void Game::printGameInfo() {
    // print board info, cell info, link info, player info.
    std::cout << "Player info:\n";
    for (unsigned i = 0; i < players.size(); ++i) {
        std::cout << "Info for player " << i + 1 << "\n";
        if (players[i].get() == nullptr) {
            std::cout << "This player is COOKED\n";
            continue;
        }
        auto [data, viruses] = players[i]->getScore();
        std::cout << "Data: " << data << " Viruses: " << viruses << "\n";

        std::cout << "Links:\n";
        for (unsigned j = 0; j < 8; ++j) {
            LinkManager::LinkKey k{players[i].get(), j};
            std::cout << "Link " << j << " ";
            if (!linkManager->hasLink(k)) {
                std::cout << "is COOKED\n";
                continue;
            }
            int strength = linkManager->getLink(k).getStrength();

            char type =
                linkManager->getLink(k).getType() == Link::LinkType::VIRUS
                    ? 'V'
                    : 'D';
            std::cout << " strength " << strength << " type " << type << " ";
            auto [linkr, linkc] = linkManager->getLink(k).getCoords();
            std::cout << "location (" << linkr << ", " << linkc << ")\n";
        }
        std::cout << "\n";
    }

    std::cout << "Board state:\n";
    auto& b = board->getBoard();
    for (unsigned r = 1; r < b.size() - 1; ++r) {
        std::string s = "";
        for (unsigned c = 0; c < b[0].size(); ++c) {
            s += b[r][c]->cellRepresentation(this);
        }
        std::cout << s << "\n";
    }
}

Game::Game() {}

Game::~Game() {}
