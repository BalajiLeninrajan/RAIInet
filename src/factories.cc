#include "factories.h"
#include "ability.h"
#include <stdexcept>

std::unique_ptr<Ability> AbilityFactory::createPlayerAbility(char id) {
    if (id == 'F') {
        return std::make_unique<FirewallAbility>();
    } else if (id == 'D') {
        return std::make_unique<DownloadAbility>();
    } else if (id == 'L') {
        return std::make_unique<LinkBoostAbility>();
    } else if (id == 'P') {
        return std::make_unique<PolarizeAbility>();
    } else if (id == 'S') {
        return std::make_unique<ScanAbility>();
    } else if (id == 'B') {
        return std::make_unique<BadConnectionAbility>();
    } else if (id == 'Q') {
        return std::make_unique<QuantumEntanglementAbility>();
    } else if (id == 'N') {
        return std::make_unique<NetworkAbility>();
    }
    throw std::invalid_argument("Invalid ability id");
}
