#include "controller.h"

#include <algorithm>
#include <boost/program_options.hpp>
#include <exception>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include "ability.h"
#include "game.h"
#include "player.h"

using std::string;
using std::vector;

namespace po = boost::program_options;

void Controller::readLinkFile(string filename, std::vector<string> &linkList,
                              int placements) {
    std::ifstream linkFile(filename);

    if (!linkFile) {
        throw std::invalid_argument("File " + filename + " not found");
    } else {
        for (int i = 0; i < placements; ++i) {
            string placement;
            if (!(linkFile >> placement)) {
                throw std::invalid_argument(
                    "File " + filename +
                    " does not have enough link placements");
            }
            linkList.push_back(placement);
        }
    }
}

void Controller::generateRandomLinks(std::vector<string> &linkList,
                                     int placements) {
    std::vector<string> link_assignments(placements, "V");
    for (int i = 0; i < placements / 2; ++i) {
        link_assignments[i] = "D";
    }

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(link_assignments.begin(), link_assignments.end(), g);

    int max_strength = 4;
    for (int i = 0; i < placements; ++i) {
        link_assignments[i] =
            link_assignments[i] + (char)('0' + (i % max_strength) - 1);
    }

    std::shuffle(link_assignments.begin(), link_assignments.end(), g);
}

void Controller::init(int argc, char *argv[]) {
    game = std::make_unique<Game>();
    // default values
    string ability1 = "LFDPS";
    string ability2 = "LFDPS";
    std::vector<string> links1;
    std::vector<string> links2;
    const int expected_link_placements = 8;
    links1.reserve(expected_link_placements);
    links2.reserve(expected_link_placements);
    bool usingGraphics = false;

    po::options_description opts("Options");
    opts.add_options()("help,h", "Print help")(
        "ability1,a1", po::value<std::string>(), "Abilities for player 1.")(
        "ability2,a2", po::value<std::string>(), "Abilities for player 2.")(
        "link1,l1", po::value<std::string>(),
        "Link placement file for player 1.")(
        "link2,l2", po::value<string>(), "Link placement file for player 2.")(
        "graphics,g", "Optional flag enabling graphical support.");

    auto style = po::command_line_style::default_style |
                 po::command_line_style::allow_long_disguise;

    auto parser =
        po::command_line_parser(argc, argv).options(opts).style(style).run();

    po::variables_map vm;

    try {
        po::store(parser, vm);

        // parse command line args

        if (vm.count("help")) {
            std::cout << "Usage: " << argv[0] << "<options>\n\n"
                      << opts << "\n";
            return;
        }

        // raise errors for required fields
        po::notify(vm);

        // player 1 abilities
        if (vm.count("ability1")) {
            auto abilities = vm["ability1"].as<string>();
            if (abilities.length() != 5) {
                throw po::validation_error(
                    po::validation_error::invalid_option_value,
                    "Must provide 5 abilities.", "ability1");
            }
            ability1 = abilities;
            std::cout << "player 1 abilities: ";
            for (auto a : abilities) {
                std::cout << a << " ";
            }
        }
        std::cout << std::endl;

        // player 2 abilities
        if (vm.count("ability2")) {
            auto abilities = vm["ability2"].as<string>();
            if (abilities.length() != 5) {
                throw po::validation_error(
                    po::validation_error::invalid_option_value,
                    "Must provide 5 abilities.", "ability2");
            }
            // parse player 2 abilities
            ability2 = abilities;
            std::cout << "player 2 abilities: ";
            for (auto a : abilities) {
                std::cout << a << " ";
            }
        }

        // link 1
        if (vm.count("link1")) {
            string filename = vm["link1"].as<string>();
            std::cout << "link 1 file: " << filename << std::endl;
            readLinkFile(filename, links1, expected_link_placements);
        } else {
            // randomize link placements for player 1
            generateRandomLinks(links1, expected_link_placements);
        }

        // link 2
        if (vm.count("link2")) {
            string filename = vm["link2"].as<string>();
            std::cout << "link 2 file: " << filename << std::endl;
            readLinkFile(filename, links2, expected_link_placements);
        } else {
            // randomize link placements for player 2
            generateRandomLinks(links2, expected_link_placements);
        }

        // graphics
        if (vm.count("graphics")) {
            std::cout << "Using graphics" << std::endl;
            usingGraphics = true;
        }

    } catch (const po::error &e) {
        std::cerr << "Error: " << e.what() << "\nTry --help\n";
        throw std::invalid_argument("");
    }
    std::vector<string> allAbilities = {ability1, ability2};
    std::vector<std::vector<string>> allLinkPlacements = {links1, links2};
    game->startGame(2, allAbilities, allLinkPlacements);
    gameIsRunning = true;
    std::cout << "Starting game\n";
    runGameLoop();
}

void Controller::runGameLoop() {
    while (gameIsRunning) {
        string s;
        std::getline(std::cin, s);
        parseCommand(s);
    }
}

void Controller::parseCommand(const std::string &commandLine) {
    std::stringstream ss(commandLine);
    string command;
    ss >> command;

    if (command == "quit") {
        gameIsRunning = false;
    } else if (command == "move") {
        string direction;
        ss >> direction;

    } else if (command == "abilities") {
    } else if (command == "ability") {
    } else if (command == "board") {
    } else if (command == "sequence") {
    }
}
Controller::Controller() {}
Controller::~Controller() {}
