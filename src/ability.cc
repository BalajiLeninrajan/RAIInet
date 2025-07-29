#include "ability.h"

Ability::Ability(std::string name) : name(name), used(false) {}

// FirewallAbility

FirewallAbility::FirewallAbility() : Ability("Firewall") {}

void FirewallAbility::use(const std::vector<std::string> &params) {}

// DownloadAbility

DownloadAbility::DownloadAbility() : Ability("Download") {}

void DownloadAbility::use(const std::vector<std::string> &params) {}

// LinkBoostAbility

LinkBoostAbility::LinkBoostAbility() : Ability("LinkBoost") {}

void LinkBoostAbility::use(const std::vector<std::string> &params) {}

// PolarizeAbility

PolarizeAbility::PolarizeAbility() : Ability("Polarize") {}

void PolarizeAbility::use(const std::vector<std::string> &params) {}

// ScanAbility

ScanAbility::ScanAbility() : Ability("Scan") {}

void ScanAbility::use(const std::vector<std::string> &params) {}

// BadConnectionAbility

void BadConnectionAbility::use(const std::vector<std::string> &params) {}

// QuantumEntanglement

QuantumEntanglementAbility::QuantumEntanglementAbility()
    : Ability("QuantumEntanglement") {}

void QuantumEntanglementAbility::use(const std::vector<std::string> &p) {}

// PappleAbility

PappleAbility::PappleAbility() : Ability("Papple") {}

void PappleAbility::use(const std::vector<std::string> &params) {}
