#include "player.h"

#include <memory>

#include "ability.h"
#include "link.h"
#include "views.h"
#include "game.h"

Player::Player(std::vector<std::unique_ptr<Ability>> abilities,
               std::shared_ptr<LinkManager> lm, Game* game)
    : abilities{std::move(abilities)}, abilitiesUsed(0), linkManager{lm}, game{game} {}

std::pair<int, int> Player::getScore() const { return score; }

const std::vector<std::unique_ptr<Ability>>& Player::getAbilities() const {
    return abilities;
}

void Player::setScore(std::pair<int, int> newScore) { score = newScore; }

int Player::getAbilitiesUsed() const { return abilitiesUsed; }

void Player::incrementAbilityUse() { abilitiesUsed++; }

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
    View::ScoreUpdate scoreUpdate{game->getPlayerIndex(*this),
                                  getScore()};
    game->addUpdate(scoreUpdate);
    linkManager->removeLink(linkKey);
}
