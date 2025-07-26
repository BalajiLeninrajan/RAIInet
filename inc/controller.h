#pragma once
#include <memory>
#include "game.h"

#include <memory>
#include <string>
#include <vector>

// Forward declarations
class View;
class Game;

// The Controller handles user input and orchestrates the Model and View.
class Controller {
    std::unique_ptr<Game> game;  // Controller owns the Game instance.
    std::vector<std::shared_ptr<View>> views;  // Manages all active views.

   public:

    Controller();
    ~Controller();
    void runGameLoop();
    void parseCommand(const std::string &commandLine);
    void init(int argc, char* argv[]);
};
