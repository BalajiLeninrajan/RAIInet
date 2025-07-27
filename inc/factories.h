#pragma once
#include <memory>

class Ability;
class Link;

class AbilityFactory {
   public:
    static std::unique_ptr<Ability> createPlayerAbility(char id);
};

class LinkFactory {
   public:
    static std::shared_ptr<Link> createLink(std::string id,
                                            std::pair<int, int> startCoords);
};
