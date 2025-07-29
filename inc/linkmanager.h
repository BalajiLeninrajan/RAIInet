#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>

class Player;
class Link;
class Board;

class LinkManager {
    std::map<Player*, std::vector<std::unique_ptr<Link>>> linkMap;

   public:
    struct LinkKey {
        Player* player;
        int id;
    };

    LinkManager();
    void addLinksForPlayer(const std::vector<std::string>& links,
                           Player* player, Board* board);
    bool removeLink(unsigned int linkID);  // returns true if link found

    Link& getLink(LinkKey key);
};
