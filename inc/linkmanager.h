#pragma once
#include <string>
#include <vector>
#include <memory>
#include <map>
#include "link.h"
#include "board.h"

class LinkManager {
    std::map<Player*, std::vector<std::unique_ptr<Link>>> linkMap;

   public:
    LinkManager();
    void addLinksForPlayer(const std::vector<std::string> &links, Player* player, Board* board);
    bool removeLink(unsigned int linkID); // returns true if link found
    /*
     * Returns an rvalue reference to a link.
     * Used for application of decorators.
     * As a side effect, sets the selected link to NULL.
    */
    bool moveLink(unsigned int linkID);

};
