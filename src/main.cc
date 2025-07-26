#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "controller.h"

using std::string;

int main(int argc, char* argv[]) {
    Controller controller;
    controller.init(argc, argv);
}
