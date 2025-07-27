#include "factories.h"

#include <stdexcept>

#include "ability.h"
#include "link.h"

std::shared_ptr<Ability> AbilityFactory::createPlayerAbility(char id) {
    switch (id) {
        case 'F':
            return std::make_shared<FirewallAbility>();
        case 'D':
            return std::make_shared<DownloadAbility>();
        case 'L':
            return std::make_shared<LinkBoostAbility>();
        case 'P':
            return std::make_shared<PolarizeAbility>();
        case 'S':
            return std::make_shared<ScanAbility>();
        case 'B':
            return std::make_shared<BadConnectionAbility>();
        case 'Q':
            return std::make_shared<QuantumEntanglementAbility>();
        case 'p':
            return std::make_shared<PappleAbility>();
        default:
            throw std::invalid_argument("Invalid ability id");
    }
}

std::shared_ptr<Link> createLink(std::string id,
                                 std::pair<int, int> startCoords) {
    switch (id[0]) {
        case 'V':
            return std::make_shared<VirusLink>(startCoords, id[1] - '0');
        case 'D':
            return std::make_shared<DataLink>(startCoords, id[1] - '0');
        default:
            throw std::invalid_argument("Invalid link id");
    }
}
