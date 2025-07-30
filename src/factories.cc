#include "factories.h"

#include <memory>
#include <stdexcept>

#include "ability.h"
#include "link.h"

std::unique_ptr<Ability> AbilityFactory::createPlayerAbility(char id) {
    switch (id) {
        case 'F':
            return std::make_unique<FirewallAbility>();
        case 'D':
            return std::make_unique<DownloadAbility>();
        case 'L':
            return std::make_unique<LinkBoostAbility>();
        case 'P':
            return std::make_unique<PolarizeAbility>();
        case 'S':
            return std::make_unique<ScanAbility>();
        case 'Q':
            return std::make_unique<QuantumEntanglementAbility>();
        case 'p':
            return std::make_unique<PappleAbility>();
        default:
            throw std::invalid_argument("Invalid ability id");
    }
}

std::unique_ptr<Link> LinkFactory::createLink(std::string id,
                                              std::pair<int, int> startCoords,
                                              Player* owner, Board* board) {
    switch (id[0]) {
        case 'V':
            return std::make_unique<VirusLink>(startCoords, id[1] - '0', owner,
                                               board);
        case 'D':
            return std::make_unique<DataLink>(startCoords, id[1] - '0', owner,
                                              board);
        default:
            throw std::invalid_argument("Invalid link id");
    }
}
