// ability.h
#pragma once

#include <string>
#include <vector>

#include "linkmanager.h"

class Game;

/**
 * @brief Abstract base class for all player abilities (Strategy interface).
 *
 * This class defines the common interface for all special actions a player can
 * take. Concrete ability classes will implement the `use` method with their
 * specific logic.
 */
class Ability {
   protected:
    std::string name;  /**< The name of the ability. */
    bool used = false; /**< Flag indicating if the ability has been used in the
                          current turn/game. */

    /**
     * @brief Helper function to convert a character link ID to a
     * LinkManager::LinkKey.
     * @param game A const reference to the Game instance.
     * @param linkId The character ID of the link.
     * @return The LinkManager::LinkKey corresponding to the link ID.
     */
    static LinkManager::LinkKey getLinkKeyFromId(const Game &game,
                                                 const char &linkId);

   public:
    /**
     * @brief Constructor for the Ability class.
     * @param name The name of the ability.
     */
    Ability(std::string name);

    /**
     * @brief Virtual destructor for the Ability class.
     */
    virtual ~Ability() = default;

    /**
     * @brief Pure virtual function to activate the ability's effect.
     *
     * Concrete ability classes must implement this method to define their
     * specific behavior when used.
     *
     * @param game A reference to the Game instance, allowing the ability to
     * interact with game state.
     * @param params A vector of strings containing any parameters required for
     * the ability's use.
     */
    virtual void use(Game &game, const std::vector<std::string> &params) = 0;

    /**
     * @brief Checks if the ability has been used.
     * @return True if the ability has been used, false otherwise.
     */
    bool isUsed() const;

    /**
     * @brief Marks the ability as used.
     */
    void markUsed();

    /**
     * @brief Gets the name of the ability.
     * @return The name of the ability.
     */
    std::string getName() const;
};

// Concrete ability implementations

/**
 * @brief Represents the Firewall ability.
 *
 * This ability allows a player to place a Firewall on the board.
 */
class FirewallAbility : public Ability {
   public:
    /**
     * @brief Constructor for FirewallAbility.
     */
    FirewallAbility();

    /**
     * @brief Uses the Firewall ability.
     * @param game A reference to the Game instance.
     * @param params A vector of strings containing parameters (i.e.
     * coordinates).
     */
    void use(Game &game, const std::vector<std::string> &params) override;
};

/**
 * @brief Represents the Download ability.
 *
 * This ability allows a player to download an opponent's link.
 */
class DownloadAbility : public Ability {
   public:
    /**
     * @brief Constructor for DownloadAbility.
     */
    DownloadAbility();

    /**
     * @brief Uses the Download ability.
     * @param game A reference to the Game instance.
     * @param params A vector of strings containing parameters (i.e. link ID).
     */
    void use(Game &game, const std::vector<std::string> &params) override;
};

/**
 * @brief Represents the Link Boost ability.
 *
 * This ability enhances a player's link, increasing its movement.
 */
class LinkBoostAbility : public Ability {
   public:
    /**
     * @brief Constructor for LinkBoostAbility.
     */
    LinkBoostAbility();

    /**
     * @brief Uses the Link Boost ability.
     * @param game A reference to the Game instance.
     * @param params A vector of strings containing parameters (i.e. link ID).
     */
    void use(Game &game, const std::vector<std::string> &params) override;
};

/**
 * @brief Represents the Polarize ability.
 *
 * This ability changes the type of a player's link (Virus to Data, or Data to
 * Virus).
 */
class PolarizeAbility : public Ability {
   public:
    /**
     * @brief Constructor for PolarizeAbility.
     */
    PolarizeAbility();

    /**
     * @brief Uses the Polarize ability.
     * @param game A reference to the Game instance.
     * @param params A vector of strings containing parameters (i.e. link ID).
     */
    void use(Game &game, const std::vector<std::string> &params) override;
};

/**
 * @brief Represents the Scan ability.
 *
 * This ability reveals information about an opponent's link.
 */
class ScanAbility : public Ability {
   public:
    /**
     * @brief Constructor for ScanAbility.
     */
    ScanAbility();

    /**
     * @brief Uses the Scan ability.
     * @param game A reference to the Game instance.
     * @param params A vector of strings containing parameters (i.e.
     * link ID).
     */
    void use(Game &game, const std::vector<std::string> &params) override;
};

/**
 * @brief Represents the WormHole ability.
 *
 * This custom ability swaps the positions of two links on the board.
 */
class WormHoleAbility : public Ability {
   public:
    /**
     * @brief Constructor for WormHoleAbility.
     */
    WormHoleAbility();
    /**
     * @brief Uses the WormHole ability.
     * @param game A reference to the Game instance.
     * @param params A vector of strings containing parameters (i.e. IDs of two
     * links to swap).
     */
    void use(Game &game, const std::vector<std::string> &params) override;
};

/**
 * @brief Represents the Quantum Entanglement ability.
 *
 * This custom ability entangles two of a player's links, causing them to move
 * together.
 */
class QuantumEntanglementAbility : public Ability {
   public:
    /**
     * @brief Constructor for QuantumEntanglementAbility.
     */
    QuantumEntanglementAbility();

    /**
     * @brief Uses the Quantum Entanglement ability.
     * @param game A reference to the Game instance.
     * @param params A vector of strings containing parameters (i.e. IDs of two
     * links to entangle).
     */
    void use(Game &game, const std::vector<std::string> &params) override;
};

/**
 * @brief Represents the Papple ability.
 *
 * This custom ability allows a player to win the game if certain conditions are
 * met, typically by forming a specific battle formation with their links.
 */
class PappleAbility : public Ability {
   public:
    /**
     * @brief Constructor for PappleAbility.
     */
    PappleAbility();

    /**
     * @brief Uses the Papple ability.
     * @param game A reference to the Game instance.
     * @param params A vector of strings containing any necessary parameters
     * (i.e. ).
     */
    void use(Game &game, const std::vector<std::string> &params) override;
};
