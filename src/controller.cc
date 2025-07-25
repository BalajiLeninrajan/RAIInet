#include "controller.h"

#include <boost/program_options.hpp>
#include <iostream>
#include <vector>

using std::string;
using std::vector;

namespace po = boost::program_options;
void Controller::init(int argc, char* argv[]) {
    po::options_description opts("Options");
    opts.add_options()("help,h", "Print help")(
        "ability1,a1", po::value<std::string>(), "Abilities for player 1.")(
        "ability2,a2", po::value<std::string>(), "Abilities for player 2.")(
        "link1,l1", po::value<std::string>(),
        "Link placement file for player 1.")(
        "link2,l2", po::value<std::string>(),
        "Link placement file for player 2.")(
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
            auto abilities = vm["ability1"].as<vector<string>>();
            if (abilities.size() != 5) {
                throw po::validation_error(
                    po::validation_error::invalid_option_value,
                    "Must provide 5 abilities.", "ability1");
            }
            // parse player 1 abilities
            std::cout << "player 1 abilities: ";
            for (auto a : abilities) {
                std::cout << a << " ";
            }
        }
        std::cout << std::endl;

        // player 2 abilities
        if (vm.count("ability2")) {
            auto abilities = vm["ability2"].as<vector<string>>();
            if (abilities.size() != 5) {
                throw po::validation_error(
                    po::validation_error::invalid_option_value,
                    "Must provide 5 abilities.", "ability2");
            }
            // parse player 2 abilities
            std::cout << "player 2 abilities: ";
            for (auto a : abilities) {
                std::cout << a << " ";
            }
        }

        // link 1
        if (vm.count("link1")) {
            string filename = vm["link1"].as<string>();
            std::cout << "link 1 file: " << filename << std::endl;
        } else {
            // randomize link placements for player 1
        }

        // link 2
        if (vm.count("link2")) {
            string filename = vm["link2"].as<string>();
            std::cout << "link 2 file: " << filename << std::endl;
        } else {
            // randomize link placements for player 2
        }

        // graphics
        if (vm.count("graphics")) {
            std::cout << "Using graphics" << std::endl;
        }

    } catch (const po::error& e) {
        std::cerr << "Error: " << e.what() << "\nTry --help\n";
        throw std::invalid_argument("");
    }
}

Controller::Controller() {}
