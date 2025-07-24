#include <iostream>
#include <boost/program_options.hpp>
#include <vector>
#include <string>

namespace po = boost::program_options;
using std::vector;
using std::string;

int main(int argc, char* argv[]) {

    po::options_description opts("Options");
    opts.add_options()
        ("help,h", "Print help")
        ("ability1,a1", po::value<std::vector<std::string>>()->multitoken(), "5 space separated ability IDs for player 1.")
        ("ability2,a2", po::value<std::vector<std::string>>()->multitoken(), "5 space separated ability IDs for player 2.");

    auto style = po::command_line_style::default_style
           | po::command_line_style::allow_long_disguise; 

    auto parser = po::command_line_parser(argc, argv)
        .options(opts)
        .style(style)
        .run();

    po::variables_map vm;

    try {
        po::store(parser, vm);

        // parse command line args

        if (vm.count("help")) {
            std::cout << "Usage: " << argv[0] << "<options>\n\n" << opts << "\n";
            return 0;
        }

        // raise errors for required fields
        po::notify(vm); 
        
        // player 1 abilities
        if (vm.count("ability1")) {
            auto abilities = vm["ability1"].as<vector<string>>();
            if (abilities.size() != 5) {
                throw po::validation_error(
                    po::validation_error::invalid_option_value,
                    "Must provide 5 abilities.",
                    "ability1"
                );
            }
            // parse player 1 abilities
            std::cout << "player 1 abilities: ";
            for (auto a: abilities) {
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
                    "Must provide 5 abilities.",
                    "ability2"
                );
            }
            // parse player 2 abilities
            std::cout << "player 2 abilities: ";
            for (auto a: abilities) {
                std::cout << a << " ";
            }
        }
        std::cout << std::endl;

    } catch (const po::error& e) {
        std::cerr << "Error: " << e.what() << "\nTry --help\n";
        return 1;
    }
}
