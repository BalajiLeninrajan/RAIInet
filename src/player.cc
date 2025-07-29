#include "player.h"

#include <memory>

#include "link.h"

Player::Player(std::vector<std::unique_ptr<Ability>> abilities)
    : abilities{abilities} {}

std::pair<int, int> Player::getScore() const { return score; }

const std::vector<std::unique_ptr<Ability>>& Player::getAbilities() const {
    return abilities;
}

void Player::setLinkManager(std::shared_ptr<LinkManager> lm) {
    linkManager = lm;
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
