#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <functional>

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
    /*
     * Returns an rvalue reference to a link.
     * Used for application of decorators.
     * As a side effect, sets the selected link to NULL.
     */
    bool moveLink(unsigned int linkID);

    void applyDecorator(LinkKey key, 
                        std::function<std::unique_ptr<Link>(std::unique_ptr<Link>)> &decorator);

    
};
