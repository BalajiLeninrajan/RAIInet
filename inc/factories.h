// factories.h
#pragma once
#include <memory>
#include <string>
#include <utility>

class Ability;
class Link;
class Player;
class Board;

/**
 * @brief Factory for creating concrete Ability objects.
 *
 * This factory simplifies the creation of different ability types,
 * abstracting away the specifics of their instantiation.
 */
class AbilityFactory {
   public:
    /**
     * @brief Creates a unique_ptr to an Ability object based on a character ID.
     * @param id The character ID representing the type of ability to create.
     * @return A unique_ptr to the newly created Ability object.
     */
    static std::unique_ptr<Ability> createPlayerAbility(char id);
};

/**
 * @brief Factory for creating concrete Link objects.
 *
 * This factory handles the creation of various link types (e.g., VirusLink,
 * DataLink), along with their initial properties and placement on the board.
 */
class LinkFactory {
   public:
    /**
     * @brief Creates a unique_ptr to a Link object with specified properties.
     * @param id The string ID representing the type of link (e.g., "v1", "d2").
     * @param startCoords The initial coordinates (row, column) of the link.
     * @param owner A pointer to the Player who owns this link.
     * @param board A pointer to the game Board where the link will be placed.
     * @return A unique_ptr to the newly created Link object.
     */
    static std::unique_ptr<Link> createLink(std::string id,
                                            std::pair<int, int> startCoords,
                                            Player* owner, Board* board);
};
