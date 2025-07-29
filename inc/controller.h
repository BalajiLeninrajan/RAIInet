#pragma once
#include <memory>
#include <string>
#include <vector>

class Game;
class View;

// The Controller handles user input and orchestrates the Model and View.
class Controller {
    std::unique_ptr<Game> game;
    std::vector<std::shared_ptr<View>> views;
    void readLinkFile(std::string filename, std::vector<std::string> &linkList, int placements);
    void generateRandomLinks(std::vector<std::string> &linkList, int placements);
    bool gameIsRunning = false;

   public:
    Controller();
    ~Controller();
    void runGameLoop();
    void parseCommand(const std::string& commandLine);
    void init(int argc, char* argv[]);
};
