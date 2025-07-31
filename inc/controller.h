// controller.h
#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Game;
class View;
class GraphicsView;
class Player;

/**
 * @brief The Controller handles user input and orchestrates the Model (Game)
 * and View.
 *
 * It acts as the intermediary, receiving commands from the user,
 * interpreting them, updating the game state through the Game model,
 * and ensuring that the views are updated accordingly.
 */
class Controller {
    std::unique_ptr<Game>
        game; /**< A unique_ptr to the Game instance (the Model). */

    std::unordered_map<Player*, std::vector<std::unique_ptr<View>>>
        views; /**< Map of Player pointers to their associated View objects. */

    /**
     * @brief Reads link data from a specified file.
     * @param filename The name of the file to read.
     * @param linkList A vector of strings to populate with link data.
     * @param placements The number of link placements to expect.
     */
    void readLinkFile(std::string filename, std::vector<std::string>& linkList,
                      int placements);

    /**
     * @brief Generates random link data.
     * @param linkList A vector of strings to populate with generated link data.
     * @param placements The number of random links to generate.
     */
    void generateRandomLinks(std::vector<std::string>& linkList,
                             int placements);

    bool gameIsRunning =
        false; /**< Flag indicating if the main game loop is active. */

    bool usingGraphics;
    std::unique_ptr<GraphicsView> graphicsView;

   public:
    /**
     * @brief Constructor for the Controller.
     */
    Controller();

    /**
     * @brief Destructor for the Controller.
     */
    ~Controller();

    /**
     * @brief Starts and manages the main game loop, handling user input and
     * turn progression.
     */
    void runGameLoop();

    /**
     * @brief Parses and executes a given command line input from the user.
     * @param commandLine The string containing the user's command.
     */
    void parseCommand(const std::string& commandLine);

    /**
     * @brief Initializes the game based on command-line arguments.
     * @param argc The number of command-line arguments.
     * @param argv An array of C-style strings representing the command-line
     * arguments.
     */
    void init(int argc, char* argv[]);

    /**
     * @brief Notifies all registered views to update their display based on the
     * current game state.
     */
    void updateViews();

    /**
     * @brief Triggers the display of all active views.
     */
    void display();

    /**
     * @brief Clears the console output for a cleaner display.
     */
    void clearStdout();
};
