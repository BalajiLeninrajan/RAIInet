#include "factories.h"
#include <stdexcept>

std::shared_ptr<Ability> AbilityFactory::createPlayerAbility(char id) {
    if (id == 'F') {
        return std::make_shared<FirewallAbility>();
    } else if (id == 'D') {
        return std::make_shared<DownloadAbility>();
    } else if (id == 'L') {
        return std::make_shared<LinkBoostAbility>();
    } else if (id == 'P') {
        return std::make_shared<PolarizeAbility>();
    } else if (id == 'S') {
        return std::make_shared<ScanAbility>();
    } else if (id == 'B') {
        return std::make_shared<BadConnectionAbility>();
    } else if (id == 'Q') {
        return std::make_shared<QuantumEntanglementAbility>();
    } else if (id == 'N') {
        return std::make_shared<NetworkAbility>();
    }
    throw std::invalid_argument("Invalid ability id");
}
