#include "player.h"

#include <memory>

#include "ability.h"
#include "link.h"

Player::Player(std::vector<std::unique_ptr<Ability>> abilities,
               std::shared_ptr<LinkManager> lm)
    : abilities{std::move(abilities)}, linkManager{lm} {}

std::pair<int, int> Player::getScore() const { return score; }

const std::vector<std::unique_ptr<Ability>>& Player::getAbilities() const {
    return abilities;
}

void Player::download(LinkManager::LinkKey linkKey) {
    Link& link = linkManager->getLink(linkKey);
    switch (link.getType()) {
        case Link::LinkType::DATA:
            ++score.first;
            break;
        case Link::LinkType::VIRUS:
            ++score.second;
            break;
    }
    linkManager->removeLink(linkKey);
}
