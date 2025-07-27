#pragma once

#include <string>
#include <vector>

// Abstract base class for all player abilities (Strategy interface).
class Ability {
    std::string name;
    bool used = false;

   public:
    virtual ~Ability() = default;
    virtual void use(const std::vector<std::string> &params) = 0;

    bool isUsed() const;
    void markUsed();
    std::string getName() const;
};

// Concrete ability implementations
class FirewallAbility : public Ability {
    void use(const std::vector<std::string> &params) override;
};
class DownloadAbility : public Ability {
    void use(const std::vector<std::string> &params) override;
};
class LinkBoostAbility : public Ability {
    void use(const std::vector<std::string> &params) override;
};
class PolarizeAbility : public Ability {
    void use(const std::vector<std::string> &params) override;
};
class ScanAbility : public Ability {
    void use(const std::vector<std::string> &params) override;
};

// Our custom abilities
class BadConnectionAbility : public Ability {
    void use(const std::vector<std::string> &params) override;
};
class QuantumEntanglementAbility : public Ability {
    void use(const std::vector<std::string> &params) override;
};
class PappleAbility : public Ability {
    void use(const std::vector<std::string> &params) override;
};
