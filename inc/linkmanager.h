#pragma once
#include <functional>
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
        unsigned id;
    };

    LinkManager();
    void addLinksForPlayer(const std::vector<std::string>& links,
                           Player* player, Board* board);

    // returns true if link found
    bool removeLink(LinkKey key);

    bool hasLink(LinkKey key);

    Link& getLink(LinkKey key);

    /*
     * returns true if the player p existed.
     */
    bool cleanPlayer(Player* p);

    /*
    * returns true if all of player p's links are deleted.
    */
    bool playerIsEmpty(Player* p);

    bool applyDecorator(
        LinkKey key,
        std::function<std::unique_ptr<Link>(std::unique_ptr<Link>)>& decorator);
};
