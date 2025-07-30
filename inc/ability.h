#pragma once

#include <string>
#include <vector>

class Game;
class LinkManager;

// Abstract base class for all player abilities (Strategy interface).
class Ability {
   protected:
    std::string name;
    bool used = false;

   public:
    Ability(std::string name);
    virtual ~Ability() = default;
    virtual void use(Game &game, LinkManager &LinkManager,
                     const std::vector<std::string> &params) = 0;

    bool isUsed() const;
    void markUsed();
    std::string getName() const;
};

// Concrete ability implementations
class FirewallAbility : public Ability {
   public:
    FirewallAbility();
    void use(Game &game, LinkManager &linkManager,
             const std::vector<std::string> &params) override;
};
class DownloadAbility : public Ability {
    DownloadAbility();
    void use(Game &game, LinkManager &linkManager,
             const std::vector<std::string> &params) override;
};
class LinkBoostAbility : public Ability {
    LinkBoostAbility();
    void use(Game &game, LinkManager &linkManager,
             const std::vector<std::string> &params) override;
};
class PolarizeAbility : public Ability {
    PolarizeAbility();
    void use(Game &game, LinkManager &linkManager,
             const std::vector<std::string> &params) override;
};
class ScanAbility : public Ability {
    ScanAbility();
    void use(Game &game, LinkManager &linkManager,
             const std::vector<std::string> &params) override;
};

// Our custom abilities
class BadConnectionAbility : public Ability {
    void use(Game &game, LinkManager &linkManager,
             const std::vector<std::string> &params) override;
};
class QuantumEntanglementAbility : public Ability {
    QuantumEntanglementAbility();
    void use(Game &game, LinkManager &linkManager,
             const std::vector<std::string> &params) override;
};
class PappleAbility : public Ability {
    PappleAbility();
    void use(Game &game, LinkManager &linkManager,
             const std::vector<std::string> &params) override;
};
