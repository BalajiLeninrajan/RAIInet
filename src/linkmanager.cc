#include "linkmanager.h"

#include "board.h"
#include "link.h"

using std::string;
using std::vector;

void LinkManager::addLinksForPlayer(const std::vector<std::string>& links,
                                    Player* player, Board* board) {
    linkMap[player] = vector<std::unique_ptr<Link>>();
    for (auto s : links) {
        // assume placement is of format DX or VX, where D/V indicates data
        // or virus, X is strength.
        // Implicit assumption that X is a digit ('0'<=X<='9')
        int strength = s[1] - '0';
        if (s[0] == 'D') {
            std::pair<int, int> sentinel = {0, 0};
            auto k =
                std::make_unique<DataLink>(sentinel, strength, player, board);
            linkMap[player].emplace_back(std::move(k));
        }
    }
}

Link& LinkManager::getLink(LinkKey key) { return *linkMap[key.player][key.id]; }

bool LinkManager::removeLink(LinkKey key) { return false; }
