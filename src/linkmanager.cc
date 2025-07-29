#include "linkmanager.h"

#include <stdexcept>

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

Link& LinkManager::getLink(LinkKey key) {
    if (!hasLink(key)) throw std::invalid_argument("Link does not exist");
    return *linkMap[key.player][key.id];
}

bool LinkManager::applyDecorator(
    LinkKey key,
    std::function<std::unique_ptr<Link>(std::unique_ptr<Link>)>& decorator) {
    if (!hasLink(key)) return false;

    linkMap[key.player][key.id] =
        decorator(std::move(linkMap[key.player][key.id]));
    return true;
}

bool LinkManager::hasLink(LinkKey key) {
    if (linkMap.find(key.player) == linkMap.end()) {
        return false;
    }

    if (key.id >= linkMap[key.player].size() || key.id < 0) {
        return false;
    }

    return linkMap[key.player][key.id] != nullptr;
}

bool LinkManager::removeLink(LinkKey key) {
    if (!hasLink(key)) return false;
    linkMap[key.player][key.id] = nullptr;
    return true;
}
