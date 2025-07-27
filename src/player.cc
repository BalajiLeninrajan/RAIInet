#include "player.h"

#include <algorithm>

#include "link.h"

Player::Player(std::vector<std::unique_ptr<Ability>> abilities,
               std::vector<std::shared_ptr<Link>> links)
    : score({0, 0}), abilities(abilities), links(links) {}

std::pair<int, int> Player::getScore() const { return score; }

const std::vector<std::unique_ptr<Ability>>& Player::getAbilities() const {
    return abilities;
}

void Player::download(Link& link) {
    switch (link.getType()) {
        case Link::LinkType::DATA:
            ++score.first;
            break;
        case Link::LinkType::VIRUS:
            ++score.second;
            break;
    }
    link.getOwner()->deleteLink(link);
}

void Player::deleteLink(Link& link) {
    links.erase(std::find(links.begin(), links.end(), link));
}

std::weak_ptr<Link> Player::getLink(int linkId) { return links[linkId]; }
