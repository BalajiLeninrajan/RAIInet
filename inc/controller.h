#pragma once
#include <gtkmm.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Game;
class View;
class Player;

// The Controller handles user input and orchestrates the Model and View.
class Controller {
    std::unique_ptr<Game> game;
    Glib::RefPtr<Gtk::Application> app;  // GTK application (only if graphics)

    std::unordered_map<Player*, std::vector<std::unique_ptr<View>>> views;
    void readLinkFile(std::string filename, std::vector<std::string>& linkList,
                      int placements);
    void generateRandomLinks(std::vector<std::string>& linkList,
                             int placements);
    bool gameIsRunning = false;

   public:
    Controller();
    ~Controller();
    void runGameLoop();
    void parseCommand(const std::string& commandLine);
    void init(int argc, char* argv[]);
    void updateViews();
    void display();
};
