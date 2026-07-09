#ifndef COLORS_HPP
#define COLORS_HPP

#include <string>

// Colors namespace to define ANSI color codes for console output
namespace Colors {
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string CYAN = "\033[36m";
    const std::string RESET = "\033[0m";
}

#endif