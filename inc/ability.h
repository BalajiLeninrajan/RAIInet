#pragma once

#include <string>
#include <vector>

#include "linkmanager.h"

class Game;

// Abstract base class for all player abilities (Strategy interface).
class Ability {
   protected:
    std::string name;
    bool used = false;

    static LinkManager::LinkKey getLinkKeyFromId(const Game &game,
                                                 const char &linkId);

   public:
    Ability(std::string name);
    virtual ~Ability() = default;
    virtual void use(Game &game, const std::vector<std::string> &params) = 0;

    bool isUsed() const;
    void markUsed();
    std::string getName() const;
};

// Concrete ability implementations
class FirewallAbility : public Ability {
   public:
    FirewallAbility();
    void use(Game &game, const std::vector<std::string> &params) override;
};
class DownloadAbility : public Ability {
   public:
    DownloadAbility();
    void use(Game &game, const std::vector<std::string> &params) override;
};
class LinkBoostAbility : public Ability {
   public:
    LinkBoostAbility();
    void use(Game &game, const std::vector<std::string> &params) override;
};
class PolarizeAbility : public Ability {
   public:
    PolarizeAbility();
    void use(Game &game, const std::vector<std::string> &params) override;
};
class ScanAbility : public Ability {
   public:
    ScanAbility();
    void use(Game &game, const std::vector<std::string> &params) override;
};

// Our custom abilities
class BadConnectionAbility : public Ability {
   public:
    void use(Game &game, const std::vector<std::string> &params) override;
};
class QuantumEntanglementAbility : public Ability {
   public:
    QuantumEntanglementAbility();
    void use(Game &game, const std::vector<std::string> &params) override;
};
class PappleAbility : public Ability {
   public:
    PappleAbility();
    void use(Game &game, const std::vector<std::string> &params) override;
};
